#ifndef __SYNTRON_SERVO_H__
#define __SYNTRON_SERVO_H__

#include <array>
#include <thread>
#include <queue>
#include <condition_variable>
#include <atomic>

#include <cmath>
#include <assert.h>
#include "motor_lift/can_motor.h"


namespace rpp
{
enum SyntronCanCode : uint16_t
{
  REVERSE_A_COUNT_DIRECTION = 0x2210,
  PARAM_WRITE_FLASH = 0x450A,
  ALARM = 0x4602,
  ALARM_CODE = 0x5012,
  RESET = 0x4603,
  PARAM_LOCK = 0x4600,
  VELOCITY = 0x5000,
  // 对应0.04m/s的速度
  VELOCITY_MAX = 2890,
  VELOCITY_MIN = 0,
};

class Syntron_Servo
{
public:
  Syntron_Servo(std::string canport, char canid);
  ~Syntron_Servo();
  void start();
  void stop();
  void setPosition(float cm);
  void setDegree(float degree);
  float getDegree(void);
  void setVelocity(float rpm);
  void setStop(void);
  void setHighLimit(float cm);
  void setLowLimit(float cm);
  bool getHighLimit(float& cm);
  bool getLowLimit(float& cm);
  bool setZeroPoint();
  void resetDriver();

  float velocity;
  float position, highlimit, lowlimit;
  // 电机是否到位
  std::atomic<bool> inplace, running;

private:
  // can端口对象
  CanMotor canmotor;
  char can_id;
  std::atomic<bool> update_loop_;

  std::thread* update_thread_;
  // can锁，避免同时操作
  std::recursive_mutex can_mutex;

  bool getEncoderPosition(int32_t& pos);
  bool getVelocity(float& rpm);
  // 设置位置到达阈值，避免抖动
  void setPositionWindow(void);
  void update_spin();
  void SetPositionMode();
  bool reversePositionCountDirection();
  void enableWriteFlash(bool enable = true);
  bool inplaceCheck();
  bool getPosition(float& cm);
  // 解锁对于重要参数的修改
  void paramLock(bool paramlock);
  void clearAlarm(void);
  bool checkAlarm(void);

  float pos2cm(int32_t pos);
  int32_t cm2pos(float cm);
  float pos2deg(int32_t pos);
  float cm2deg(float cm);
  float deg2cm(float deg);
  int32_t deg2pos(float deg);
};

}  // namespace rpp

#endif  // __H_RPP_DRIVER_H__
