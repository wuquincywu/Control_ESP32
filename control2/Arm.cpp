#include "Arm.h"
#include <math.h>
#include <driver/ledc.h>

#define pi 3.1415926

Arm::Arm(uint8_t arm1_Pin, uint8_t arm2_Pin, uint8_t rotation_Pin)
    : arm1_Pin(arm1_Pin), arm2_Pin(arm2_Pin), rotation_Pin(rotation_Pin) {}

void Arm::setup() {
    
    pinMode(arm1_Pin, OUTPUT);
    pinMode(arm2_Pin, OUTPUT);
    pinMode(rotation_Pin, OUTPUT);

    ledcSetup(8, 50, 8);
    ledcAttachPin(arm1_Pin, 8);
    ledcSetup(9, 50, 8);
    ledcAttachPin(arm2_Pin, 9);
    ledcSetup(10, 50, 8);
    ledcAttachPin(rotation_Pin, 10);

    arm1_Servo.attach(arm1_Pin, 544, 2400);
    arm2_Servo.attach(arm2_Pin, 544, 2400);
    rotation_Servo.attach(rotation_Pin, 544, 2400);
    
    updateServos();
    rotation_Servo.writeMicroseconds(STOP_PULSE);
}

void Arm::update(const ControlState& input) {
    if (input.homePressed) isHoming = true; 
    if (isHoming) { goHome(); return; }

    // 處理末端效果器X軸移動 (R1/L1)
    if (input.r1Pressed) currentX += maxSpeed;
    if (input.l1Pressed) currentX -= maxSpeed;
    
    // 處理末端效果器Y軸移動 (D-pad上下)
    if (input.dpadY > 0)  currentY += maxSpeed;
    if (input.dpadY < 0)  currentY -= maxSpeed;
    
    

    // 限制工作空間範圍 (根據機械結構調整)
    currentX = constrain(currentX, -400.0, -50.0);
    currentY = constrain(currentY, -120.0, 300.0);
    
    // 計算逆運動學
    solveInverseKinematics(currentX, currentY);
    
    // 更新旋轉伺服 (D-pad左右)
    setRotation(input.dpadX);
    
    // 更新伺服位置
    updateServos();
}

void Arm::solveInverseKinematics(float x, float y) {
    // 逆運動學計算 
    double s1 = sqrt((x*x+y*y));
    double t0 = atan(-y/x);
    double t1 = acos((s1*s1+l1*l1-l41*l41)/(2*s1*l1));
    double t2 = acos((s1*s1+l41*l41-l1*l1)/(2*s1*l41));
    double s2 = sqrt(s1*s1 +l4*l4 -2*s1*l4*cos(t2));
    double t3 = acos((l1*l1+s2*s2-l42*l42)/(2*s2*l1));
    double t4 = acos((l2*l2+s2*s2-l3*l3)/(2*s2*l2));
    double theta1 = pi -t0 -t1;
    double theta2 = theta1 -t3 -t4;

    // 轉換為角度 (弧度轉度數)
    arm1_Angle = degrees(theta1);
    arm2_Angle = degrees(theta2);
}

void Arm::setRotation(int direction) {
    // 更新旋轉位置
    static int pulse = STOP_PULSE;

    switch(direction){
        case(1): pulse = CCW_PULSE;break;
        case(0): pulse = STOP_PULSE;break;
        case(-1): pulse = CW_PULSE; break;
    }
    rotation_Servo.writeMicroseconds(pulse);
}

void Arm::updateServos() {
    if (!isnan(arm2_Angle)){
        float angle1 = arm1_Angle + arm1_offset;
        float angle2 = -arm2_Angle + arm2_offset;
        //angle2 = constrain(angle2,)
        arm1_Servo.write(angle1);
        arm2_Servo.write(angle2);
    }
}

void Arm::setPosition(float x, float y) {
    targetX = x;
    targetY = y;
}

int Arm::getRotationState() const {
    return rotationDirection ;
}

void Arm::printArmState() const {
    Serial.print("手臂位置: (");
    Serial.print(currentX, 1); // 保留1位小数
    Serial.print(", ");
    Serial.print(currentY, 1);
    Serial.print(") | 角度: ");
    Serial.print(arm1_Angle, 3); // 整数显示
    Serial.print("°, ");
    Serial.print(arm2_Angle, 3);
    Serial.print("° | 旋转: ");
    
    switch(getRotationState()) {
        case -1: Serial.print("逆时针"); break;
        case 0: Serial.print("停止"); break;
        case 1: Serial.print("顺时针"); break;
    }
}  

void Arm::goHome(){
    float err1 = arm1_Angle - homeArmAngle1;
    float err2 = arm2_Angle - homeArmAngle2;
    if (fabs(err1) > 1e-3) {
        if(fabs(err1) < maxHomeAngle){
            arm1_Angle = homeArmAngle1;
        }
        else{
            arm1_Angle += (err1 < 0 ? +maxHomeAngle : -maxHomeAngle);
        }
    }
    if(fabs(err2) < maxHomeAngle){
            arm2_Angle = homeArmAngle2;
    }        
    else{
        arm2_Angle += (err2 < 0 ? +maxHomeAngle : -maxHomeAngle);
    }
    
    updateServos();
    if (fabs(err1) < 1e-3 &&
        fabs(err2) < 1e-3) {
        isHoming = false;
        currentX = homeX;
        currentY = homeY; 
    }
}