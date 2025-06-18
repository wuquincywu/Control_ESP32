#include "OutputManager.h"

OutputManager::OutputManager(Arm& arm, Jensen& chassis, Dualsense& controller)
    : _arm(arm), _chassis(chassis), _controller(controller) {}

void OutputManager::begin() {
    Serial.begin(115200);
    Serial.println("系统启动 - 准备输出状态...");
}

void OutputManager::update() {
    unsigned long currentTime = millis();
    
    
        
         Serial.println("\n===== 系统状态 =====");
        
        // 输出手臂状态
        _arm.printArmState();
        Serial.println();
        
        // 输出底盘状态
       // _chassis.printChassisState();
        //Serial.println();
        
        // 输出完整ControlState
        Serial.println("控制器状态:");
        ControlState state = _controller.getInput();
        Serial.print("左摇杆: "); Serial.print(state.leftSpeed, 2);
        Serial.print(" | 右摇杆: "); Serial.print(state.rightSpeed, 2);
        Serial.print(" | 左扳机: "); Serial.print(state.leftTriggerSpeed, 2);
        Serial.print(" | 右扳机: "); Serial.println(state.rightTriggerSpeed, 2);
        
        Serial.print("D-pad: X="); Serial.print(state.dpadX);
        Serial.print(" Y="); Serial.print(state.dpadY);
        Serial.print(" | L1: "); Serial.print(state.l1Pressed ? "按下" : "释放");
        Serial.print(" | R1: "); Serial.println(state.r1Pressed ? "按下" : "释放");
        
        Serial.println("===================");
        
    
}