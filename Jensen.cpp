#include "Jensen.h"

Jensen::Jensen(uint8_t ena, uint8_t in1, uint8_t in2,
               uint8_t in3, uint8_t in4, uint8_t enb)
    : _ena(ena), _in1(in1), _in2(in2),
      _in3(in3), _in4(in4), _enb(enb) {}

void Jensen::setup() {
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
    pinMode(_ena, OUTPUT);
    pinMode(_in3, OUTPUT);
    pinMode(_in4, OUTPUT);
    pinMode(_enb, OUTPUT);

    stop();
    setMaxSpeed(_maxSpeed);


    ledcSetup(chA, freq, res);
    ledcSetup(chB, freq, res);
    ledcAttachPin(_ena, chA);
    ledcAttachPin(_enb, chB);
    ledc_fade_func_install(LEDC_LOW_SPEED_MODE);

}

void Jensen::stop() {
    _currentLeftSpeed = _currentRightSpeed = 0;
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, LOW);
    digitalWrite(_in3, LOW);
    digitalWrite(_in4, LOW);

    ledcWrite(chA, 0);
    ledcWrite(chB, 0);

}

void Jensen::update(const ControlState& input) {
    _currentLeftSpeed  = constrain(input.leftSpeed,  -1.0f, 1.0f) * (-_maxSpeed);
    _currentRightSpeed = constrain(input.rightSpeed, -1.0f, 1.0f) * (-_maxSpeed);

    

    setMotor(_in1, _in2, _ena, _currentLeftSpeed);
    setMotor(_in3, _in4, _enb, _currentRightSpeed);
}

void Jensen::setMaxSpeed(float m) {
    _maxSpeed = constrain(m, 0.1f, 1.0f);
}
    
void Jensen::setMotor(uint8_t in1, uint8_t in2, uint8_t en, float speed) {
    // direction
    int dir = 0 ;
    if      (speed > 0) { digitalWrite(in1, HIGH); digitalWrite(in2, LOW);  dir = 1;}
    else if (speed < 0) { digitalWrite(in1, LOW);  digitalWrite(in2, HIGH); dir = 2;}
    else                { digitalWrite(in1, LOW);  digitalWrite(in2, LOW);  dir = 3;}

    // PWM duty

    int duty = int(fabs(speed) * ((1 << res) - 1));
    int channel = (en == _ena ? chA : chB);
    ledcWrite(channel, duty);
    /*
    if (en == _ena){
        Serial.print("left : "); Serial.println(dir);
    }
    else{
        Serial.print("right : "); Serial.println(dir);
    }
    */
}