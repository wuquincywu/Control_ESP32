#include "Controller.h"
#include <Bluepad32.h>

// 静态成员初始化
Dualsense* Dualsense::instance = nullptr;

void Dualsense::setup() {
  
  instance = this;
  BP32.setup(&onControllerConnected, &onControllerDisconnected);
}

void Dualsense::update() {
  BP32.update();
  
  if (myController && myController->isConnected()) {
    // 重置状态
    //controlState = getInput();
    
    // 处理模拟输入
    processAnalogInputs();
    
    // 处理方向键
    processDpadInput();
    
    // 处理按钮
    processButtons();
  }
}

bool Dualsense::isConnected() const {
  return myController != nullptr && myController->isConnected();
}

const ControlState& Dualsense::getInput() const {
  return controlState;
}

void Dualsense::processAnalogInputs() {
  // 摇杆输入 (-511~512 映射到 -1.0~1.0)
  controlState.leftSpeed = myController->axisY() / -512.0f;
  controlState.rightSpeed = myController->axisRY() / -512.0f;
  
  // 线性扳机输入 (0~1023 映射到 0.0~1.0)
  controlState.leftTriggerSpeed = myController->brake() / 1023.0f;
  controlState.rightTriggerSpeed = myController->throttle() / 1023.0f;
  
  // 应用死区过滤
  if (fabs(controlState.leftSpeed) < 0.1) controlState.leftSpeed = 0;
  if (fabs(controlState.rightSpeed) < 0.1) controlState.rightSpeed = 0;
  if (controlState.leftTriggerSpeed < 0.1) controlState.leftTriggerSpeed = 0;
  if (controlState.rightTriggerSpeed < 0.1) controlState.rightTriggerSpeed = 0;
}

void Dualsense::processDpadInput() {
  // 方向键位掩码
  const uint8_t DPAD_UP    = 0x01;
  const uint8_t DPAD_DOWN  = 0x02;
  const uint8_t DPAD_LEFT  = 0x04;
  const uint8_t DPAD_RIGHT = 0x08;
  
  uint8_t dpad = myController->dpad();
  
  controlState.dpadX = 0;
  controlState.dpadY = 0;
  
  if (dpad & DPAD_UP) controlState.dpadY = 1;
  if (dpad & DPAD_DOWN) controlState.dpadY = -1;
  if (dpad & DPAD_LEFT) controlState.dpadX = -1;
  if (dpad & DPAD_RIGHT) controlState.dpadX = 1;
}

void Dualsense::processButtons() {
  controlState.r1Pressed = myController->r1();
  controlState.l1Pressed = myController->l1();

  static const uint16_t BUTTON_SQUARE = 0x0004; // 方形按鈕 (X)
  static const uint16_t BUTTON_TRIANGLE = 0x0002; // 三角形按鈕 (Y)
  static const uint16_t BUTTON_PS = 0x01; // PS按鈕 
  uint16_t buttons = myController->buttons();
  uint16_t misc = myController->miscButtons();
  controlState.squarePressed = buttons & BUTTON_SQUARE;
  controlState.trianglePressed = buttons & BUTTON_TRIANGLE;
  controlState.homePressed = misc & BUTTON_PS;

}

void Dualsense::updateControllerFeedback() {
  if (!myController || !myController->isConnected()) return;
  
  // 根据连接状态设置LED
  static bool ledState = false;
  ledState = !ledState;

  myController->setColorLED(ledState ? 0 : 255, ledState ? 255 : 0, 0);
    
    // 根据扳机压力设置阻力反馈
  /*if (controlState.rightTriggerSpeed > 0.3f) {
      myController->setTriggerEffect(RIGHT_TRIGGER, DS5_TRIGGER_EFFECT_MODE_ADVANCED_RESISTANCE, 
                                     0, 255 * controlState.rightTriggerSpeed);
  }
  if (controlState.leftTriggerSpeed > 0.3f) {
      myController->setTriggerEffect(LEFT_TRIGGER, DS5_TRIGGER_EFFECT_MODE_ADVANCED_RESISTANCE, 
                                     0, 255 * controlState.lefttTriggerSpeed);
  }*/
}
  


void Dualsense::onControllerConnected(ControllerPtr ctl) {
  if (instance) {
    instance->myController = ctl;
    Serial.println("控制器已连接");
  }
}

void Dualsense::onControllerDisconnected(ControllerPtr ctl) {
  instance->myController = ctl;
  Serial.println("控制器未連接");
}