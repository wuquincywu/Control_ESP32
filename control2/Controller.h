#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Bluepad32.h>

struct ControlState {
  // 摇杆和扳机输入
  float leftSpeed = 0.0;    // 左摇杆Y轴 (-1.0~1.0)
  float rightSpeed = 0.0;   // 右摇杆Y轴 (-1.0~1.0)
  float leftTriggerSpeed = 0.0; // 线性扳机输入 (0.0~1.0)
  float rightTriggerSpeed = 0.0; // 线性扳机输入 (0.0~1.0)
  
  // 方向键
  int dpadX = 0;            // -1, 0, 1
  int dpadY = 0;            // -1, 0, 1
  
  // 按钮状态
  bool r1Pressed = false;
  bool l1Pressed = false;

  bool squarePressed = false;
  bool trianglePressed = false;
  bool homePressed = false;
};

class Dualsense {
public:
  static Dualsense* instance;

  void setup();
  void update();
  bool isConnected() const;
  const ControlState& getInput() const;
  
private:
  void processAnalogInputs();
  void processDpadInput();
  void processButtons();
  void updateControllerFeedback();

  static void onControllerConnected(ControllerPtr ctl);
  static void onControllerDisconnected(ControllerPtr ctl);
  
  ControllerPtr myController = nullptr;
  ControlState controlState;
  
  
  
};

#endif