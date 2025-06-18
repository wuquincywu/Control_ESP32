#include "Controller.h"
#include "Arm.h"
#include "Jensen.h"
#include "Gripper.h"
#include "OutputManager.h"

Dualsense controller;
Arm arm(19,21,22); // 大，小，轉
Jensen jensen( 14, 27, 26, 25, 33,32);// ena in1 in2 in3 in4 enb  left right
Gripper gripper (23);
OutputManager output(arm, jensen, controller);

// 藍芽狀態燈
const uint8_t LED_PIN = 2;
unsigned long lastLEDupdate = 0;
const unsigned long BLINK_INTERVAL = 500;
bool ledState = LOW;

unsigned long lastControllerUpdate = 0;
const unsigned long CONTROLLER_UPDATE_INTERVAL = 10; // 10ms
unsigned long lastArmUpdate = 0;
const unsigned long ARM_UPDATE_INTERVAL = 20; // 20ms
unsigned long lastJensenUpdate = 0;
const unsigned long JENSEN_UPDATE_INTERVAL = 20; // 20ms
unsigned long lastGripperUpdate = 0;
const unsigned long GRIPPER_UPDATE_INTERVAL = 20; // 20ms
unsigned long lastOutputUpdate = 0;
const unsigned long OUTPUT_UPDATE_INTERVAL = 100; // 100ms


void setup() {

  Serial.begin(115200);
  
  // 初始化各模块
  controller.setup();
  arm.setup();
  jensen.setup();
  gripper.setup();
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
  Serial.println("系统初始化完成");
}

void loop() {
    unsigned long currentTime = millis();
    //Serial.println(currentTime);
    
    // 更新控制器状态（最高優先級）
    if (currentTime - lastControllerUpdate >= CONTROLLER_UPDATE_INTERVAL) {
        controller.update();
        //Serial.println(currentTime - lastControllerUpdate);
        lastControllerUpdate = currentTime;
        
    }
    
    // 如果控制器已连接
    if (controller.isConnected()) {

        digitalWrite(LED_PIN, HIGH);
        lastLEDupdate = millis();
        ledState   =  HIGH ;

        ControlState input = controller.getInput();
        
        // 更新手臂位置
        if (currentTime - lastArmUpdate >= ARM_UPDATE_INTERVAL) {
            arm.update(input);
            //Serial.println(currentTime - lastArmUpdate);
            lastArmUpdate = currentTime;
        }
        
        // 更新底盘控制
        if (currentTime - lastJensenUpdate >= JENSEN_UPDATE_INTERVAL) {
            jensen.update(input);
            lastJensenUpdate = currentTime;
        }
        
        // 更新夹爪
        if (currentTime - lastGripperUpdate >= GRIPPER_UPDATE_INTERVAL) {
            gripper.update(input);
            lastGripperUpdate = currentTime;
        }
        
        if (currentTime - lastOutputUpdate >= OUTPUT_UPDATE_INTERVAL) {
        //output.update();
            lastOutputUpdate = currentTime;
        }
    }
    else{
        if (currentTime - lastLEDupdate >= BLINK_INTERVAL) {
        lastLEDupdate = currentTime;
        ledState   = !ledState;
        digitalWrite(LED_PIN, ledState);
        }
    }
    delay(1);
}