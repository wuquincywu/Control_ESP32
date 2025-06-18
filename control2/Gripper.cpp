#include "Gripper.h"
#include <Arduino.h>

Gripper::Gripper(int pin) : _pin(pin) {}

void Gripper::setup() {
    
    pinMode(_pin, OUTPUT);
    
    ledcSetup(11, 50, 8);
    ledcAttachPin(_pin, 11);
    
    _servo.attach(_pin, 544, 2400);
    
    _servo.writeMicroseconds(STOP_PULSE);
    Serial.println("Gripper initialized with dedicated timer");
}

Gripper::State Gripper::getState() const {
    return _state;
}

void Gripper::update(const ControlState& input) {
    
    // 按钮优先级：最后按下的按钮优先
    static bool lastSquare = false;
    static bool lastTriangle = false;
    
    // 检测按钮按下（上升沿）
    if (input.squarePressed) {
        _servo.writeMicroseconds(CLOSE_PULSE);
        _state = CLOSING;
        
    }
    else if (input.trianglePressed) {
        _servo.writeMicroseconds(OPEN_PULSE);
        _state = OPENING;
    }
    else{
      _servo.writeMicroseconds(STOP_PULSE);
        _state = STOPPED;
    }
}