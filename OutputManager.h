#ifndef OUTPUT_MANAGER_H
#define OUTPUT_MANAGER_H

#include <Arduino.h>
#include "Controller.h"
#include "Arm.h"
#include "Jensen.h"

class OutputManager {
public:
    OutputManager(Arm& arm, Jensen& chassis, Dualsense& controller);
    
    void begin();
    void update();
    
private:
    Arm& _arm;
    Jensen& _chassis;
    Dualsense& _controller;
    
    
};

#endif