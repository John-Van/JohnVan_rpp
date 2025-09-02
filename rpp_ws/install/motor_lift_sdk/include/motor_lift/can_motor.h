#ifndef CAN_MOTOR_H
#define CAN_MOTOR_H

#include <iostream>         // 包含输入输出流，用于打印日志或调试信息
#include <string>           // 包含 string 类的定义，用于字符串操作
#include <cstring>          // 包含 C 风格字符串操作函数的定义
#include <unistd.h>         // 包含 POSIX 系统调用，例如 close, read, write
#include <net/if.h>         // 包含网络接口相关的定义
#include <sys/ioctl.h>      // 包含 I/O 控制相关的函数定义，例如 SIOCGIFINDEX
#include <fcntl.h>          // 包含文件控制相关的定义
#include <linux/can.h>      // 包含 CAN 协议相关的定义
#include <linux/can/raw.h>  // 包含原始 CAN 套接字定义
#include <sys/socket.h>     // 包含 socket 套接字的相关定义
#include <mutex>            // 包含互斥量的定义，用于线程同步
#include <thread>

namespace rpp
{

// CAN 通信类，用于发送和接收 CAN 消息
class CanCommunication
{
public:
  // 构造函数，初始化 CAN 通信
  CanCommunication(const std::string& interfaceName);
  // 析构函数，关闭 CAN 套接字
  ~CanCommunication();
  // 发送 CAN 消息
  void sendCanMessage(const can_frame& frame);
  // 接收 CAN 消息
  int receiveCanMessages(can_frame& cf);

  void activeAllNodes(void);
  void activate(uint8_t CANopen_ID);
  can_frame SDO_Write(uint8_t CANopen_ID, uint8_t CMD, uint16_t Index, uint8_t SubIndex, uint32_t DATA);
  bool SDO_Recv(uint8_t CANopen_ID, can_frame& cf);
  bool checkSDORecv(can_frame cf, uint16_t Index);
  // 根据各电机协议自行修改
  void parse_error_code(can_frame cf);

private:
  int sock;  // 套接字描述符
};

// CAN 电机类，继承自 CanCommunication，提供电机控制功能
class CanMotor : public CanCommunication
{
public:
  CanMotor(const std::string& interfaceName) : CanCommunication(interfaceName)
  {
  }
  ~CanMotor()
  {
  }
  void SetControlMode(uint8_t CANopen_ID, uint8_t CANopen_mode);
  void Enable(uint8_t CANopen_ID);
  void Disable(uint8_t CANopen_ID);
  // 单位为脉冲 10000,常规电机转1圈
  void SetTargetPosition(uint8_t CANopen_ID, int32_t pos);
  // 设置速度 0.1 rpm
  void SetProfileVelocity(uint8_t CANopen_ID, uint32_t vel);
  void PPIncrementalTrigger(uint8_t CANopen_ID);
  void PPAbsoluteTrigger(uint8_t CANopen_ID);
  bool GetPositionActualValue(uint8_t CANopen_ID, int32_t& pos);
  bool GetVelocityActualValue(uint8_t CANopen_ID, int32_t& vel);
  bool GetVelocitySensorActualValue(uint8_t CANopen_ID, int32_t& vel);
  bool isMotorAtPosition(uint8_t CANopen_ID);
  void setEmergencyStop(uint8_t CANopen_ID);
  void resetEmergencyStop(uint8_t CANopen_ID);
  void setHomeOffset(uint8_t CANopen_ID, int32_t pos);
  bool getHomeOffset(uint8_t CANopen_ID, int32_t& pos);
  void setHomingMethod(uint8_t CANopen_ID, int8_t hm);
  void setLowSoftPositionLimit(uint8_t CANopen_ID, int32_t pos);
  void setHighSoftPositionLimit(uint8_t CANopen_ID, int32_t pos);
  bool getLowSoftPositionLimit(uint8_t CANopen_ID, int32_t& pos);
  bool getHighSoftPositionLimit(uint8_t CANopen_ID, int32_t& pos);
  bool GetProfileVelocity(uint8_t CANopen_ID, uint32_t& vel);

private:
  void parse_error_code(can_frame cf);
};

enum CanCode : char
{
  SDO_W4 = 0x23,
  SDO_W2 = 0x2B,
  SDO_W1 = 0x2F,
  SDO_Read = 0x40,

  POSITION_MODE = 1,
  HOMING_MODE = 6,
  // 反向找机械限位
  ZERO_BACK_LIMIT = 17,
  // 正向找机械限位
  ZERO_FORWARD_LIMIT = 18,
  // 反向找机械零点
  ZERO_BACK_ZERO = 19,
  // 正向找机械零点
  ZERO_FORWARD_ZERE = 20,
};

enum CanDict : uint16_t
{
  CONTROL_WORD = 0X6040,
  STATUS_WORD = 0X6041,
  MODES_OF_OPERATION = 0X6060,
  MODES_0F_OPERATION_DISPLAY = 0X6061,
  POSITION_ACTUAL_VALUE = 0X6063,
  // POSITION_COMMAND_VALUE = 0X6064,

  VELOCITY_SENSOR_ACTUAL_VALUE = 0X6069,
  VELOCITY_ACTUAL_VALUE = 0X606C,
  TARGET_TORQUE = 0X6071,
  TARGET_POSITION = 0X607A,
  PROFILE_VELOCITY = 0X6081,
  PROFILE_ACCLERATION = 0X6083,
  PROFILE_DECELERATION = 0X6084,
  TORQUE_SLOPE = 0X6087,
  POSITION_FACTOR = 0X6093,
  TARGET_VELOCITY = 0X60FF,
  HOME_OFFSET = 0x607C,
  HOMING_METHOD = 0X6098,
  SOFT_POSITION_LIMIT = 0X607D,

};
}  // namespace rpp

#endif