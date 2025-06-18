#ifndef GRIPPER_H
#define GRIPPER_H

#include <Bluepad32.h>
#include <ESP32Servo.h>
#include "Controller.h"

class Gripper {
public:
    // 建構函數
    Gripper(int pin);
    
    // 初始化函數
    void setup();
    
    // 更新夾爪狀態
    void update(const ControlState& input);
    
    // 夾爪狀態
    enum State {
        OPENING,
        CLOSING,
        STOPPED
    };
    
    State getState() const;

private:
    // 舵機控制
    Servo _servo;
    int _pin;
    
    // 舵機脈衝設定
    static const int OPEN_PULSE = 1400;   // 打開夾爪 (逆時針)
    static const int CLOSE_PULSE = 1600;  // 關閉夾爪 (順時針)
    static const int STOP_PULSE = 1500;   // 停止
    
    // 夾爪狀態
    State _state = STOPPED;
    
    
};

#endif