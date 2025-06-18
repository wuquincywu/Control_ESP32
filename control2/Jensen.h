// ===== Jensen.h =====
#ifndef JENSEN_H
#define JENSEN_H

#include <Arduino.h>
#include"Controller.h"
#ifdef ESP32
  #include "driver/ledc.h"
#endif

class Jensen {
public:
    // 构造：in1/in2 控制方向，ena 输出 PWM；in3/in4/enb 同理
    Jensen(uint8_t ena, uint8_t in1, uint8_t in2,
           uint8_t in3, uint8_t in4, uint8_t enb);

    void setup();                             
    void update(const ControlState& input);
    void stop();                              
    void setMaxSpeed(float m);                

private:
    void setMotor(uint8_t in1, uint8_t in2, uint8_t en, float speed);

    uint8_t _in1, _in2, _ena, _in3, _in4, _enb;
    float _maxSpeed       = 1.0f;
    float _currentLeftSpeed  = 0.0f;
    float _currentRightSpeed = 0.0f;

#ifdef ESP32
    static constexpr int chA = 12;
    static constexpr int chB = 13;
    static constexpr int freq = 20000;
    static constexpr int res  = 10;
#endif
};

#endif // JENSEN_H