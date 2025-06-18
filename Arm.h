#ifndef ARM_H
#define ARM_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include "Controller.h"

class Arm {
public:
    Arm(uint8_t arm1_Pin, uint8_t arm2_Pin, uint8_t rotation_Pin);
    void setup();
    void update(const ControlState& input);
    void setPosition(float x, float y);
    void setRotation(int direction); // -1: CCW, 0: Stop, 1: CW

    // 傳到Output
    void printArmState() const;
    int getRotationState() const; // -1=逆时针, 0=停止, 1=顺时针

private:
    void solveInverseKinematics(float x, float y);
    void updateServos();
    void goHome();
    
    // 伺服馬達引腳
    uint8_t arm1_Pin;
    uint8_t arm2_Pin;
    uint8_t rotation_Pin;
    
    // 伺服馬達對象
    Servo arm1_Servo;
    Servo arm2_Servo;
    Servo rotation_Servo;
    
    // 手臂幾何參數 (單位: mm)
    const float l1 = 220; 
    const float l2 = 80.27; 
    const float l3 = 237.84; 
    const float l4 = 280; 
    const float l41 = 216.92; 
    const float l42 = 63.08; 
    
    // 當前位置
    float currentX = -160;
    float currentY = 100.0; 
    int rotationDirection = 0; 
    
    // 目標位置
    float targetX = -160;
    float targetY = 100.0;
    
    // 伺服角度
    float arm1_offset = 7;
    float arm2_offset = 85;
    float arm1_Angle = 84.4;   
    float arm2_Angle = 11.3; 
    
    // 運動參數
    const float maxSpeed = 2.5;    // mm/update
    const int maxHomeAngle = 1;
    const int CW_PULSE = 1800; 
    const int STOP_PULSE = 1500;
    const int CCW_PULSE = 1200;

    // goHome() Const
    bool isHoming = false;
    const float homeArmAngle1 = 84.4;
    const float homeArmAngle2 = 11.3;
    const float homeX = -160;
    const float homeY = 100;
};

#endif