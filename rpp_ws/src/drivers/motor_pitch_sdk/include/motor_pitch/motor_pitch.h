#ifndef __TI5CRA_SERVO_H__
#define __TI5CRA_SERVO_H__

#include <array>
#include <thread>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <assert.h>
#include "can.h"

namespace rpp
{

enum Ti5CRAFunctionCode : char
{
  SET_POSITION = 0x1E,
  SET_STOP = 0x02,
  GET_POSITION = 0x08,
  GET_ENCODER_POSITION = 0x08,
  GET_VELOCITY = 0x06,
  SET_POSITION_OFFSET = 0x53,
  GET_POSITION_OFFSET = 0x54,
  GET_UPLIMIT = 0x1A,
  GET_DOWNLIMIT = 0x1B,
  SET_UPLIMIT = 0x26,
  SET_DOWNLIMIT = 0x27,
  SET_UPVELLIMIT = 0x24,
  SET_DOWNVELLIMIT = 0x25,
  GET_UPVELLIMIT = 0x18,
  GET_DOWNVELLIMIT = 0x19,
  WRITE_FLASH = 0x0E,
  VELOCITY_MAX = 10,
  VELOCITY_MIN = 0,
  SET_CURRENT_BEFORE_MOVE = 0x47,
  ERROR = 0x0A,
};

class Ti5CRA_Servo
{
public:
  Ti5CRA_Servo(std::string canport, char canid);
  ~Ti5CRA_Servo();

  void start();
  void stop();
  void moveForward(float deg = 1);
  void moveBackward(float deg = 1);

  void setPosition(float deg);
  void setVelocity(float rpm);
  void setStop(int32_t t_ms = 0);
  bool setZeroPoint();
  void setCurrentBeforeMove(float current);

  bool setHighLimit(float deg);
  bool setLowLimit(float deg);

  float velocity;
  float position;
  // 上下限位
  float highlimit, lowlimit;
  // 电机是否到位
  std::atomic<bool> inplace, running;

private:
  // can端口对象
  CanCommunication canComm;
  // CAN 消息
  can_frame sendFrame;
  char can_id;
  std::atomic<bool> update_loop_;
  std::thread* update_thread_;
  // can锁，避免同时操作
  std::recursive_mutex can_mutex;
  // 编码器数值，目标位置
  float target_position;
  // 当前设置的电流
  float current_before_move;

  void update_spin();
  void param_init();
  // 根据电机位置，设置合适的启动前电流
  void setCurrentAccordingToPosition(float position);
  bool setUpVelLimit(float rpm);
  bool setDownVelLimit(float rpm);
  bool getUpVelLimit(float& rpm);
  bool getDownVelLimit(float& rpm);
  bool getHighLimit(float& deg);
  bool getLowLimit(float& deg);
  bool getPosition(float& deg);
  bool getEncoderPosition(int32_t& pos);
  bool getVelocity(float& rpm);
  // 位置偏移
  void setPositionOffect(int32_t pos);
  bool getPositionOffset(int32_t& pos);
  void writeFlash();

  // 生成四字节的数据包
  void creat_int32t_msg(can_frame& cf, uint8_t begin_pos, int32_t num);
  // 解析四字节数据包
  int32_t parse_int32t_msg(can_frame& cf, uint8_t begin_pos);
  // 解析错误码
  void parse_error_code(can_frame& cf, uint8_t begin_pos);

  // 发送当前的发送数据包，获取四字节数据
  bool get_i32data(char cmd, int32_t& num);
  void set_i32data(char cmd, int32_t num);
  void send_byteCmd(char cmd);
  float pos2deg(int32_t pos);
  int32_t deg2pos(float deg);
  int32_t rpm2hz(float rpm);
  float hz2rpm(int32_t hz);
};

}  // namespace rpp

#endif
