#include "motor_lift/motor_lift.h"

using namespace std;

namespace rpp
{

Syntron_Servo::Syntron_Servo(string canport, char canid) : canmotor(canport)
{
  // 设置 CAN ID
  can_id = canid;
  update_thread_ = nullptr;
  setVelocity(0.02);  
  clearAlarm();
  highlimit = 0;
  lowlimit = -46;
  update_loop_ = true;
  running = false;
}

Syntron_Servo::~Syntron_Servo()
{
  if (update_thread_ != nullptr)
  {
    update_loop_ = false;
    update_thread_->join();
    delete update_thread_;
    update_thread_ = nullptr;
  }
  lock_guard<recursive_mutex> lock(can_mutex);
  // 关闭电机
  canmotor.Disable(can_id);
}

void Syntron_Servo::start()
{
  lock_guard<recursive_mutex> lock(can_mutex);
  clearAlarm();
  SetPositionMode();
  // 激活电机
  canmotor.Enable(can_id);
  update_loop_ = true;
  running = true;
  if (update_thread_ == nullptr)
    update_thread_ = new thread(&Syntron_Servo::update_spin, this);
}

void Syntron_Servo::stop()
{
  lock_guard<recursive_mutex> lock(can_mutex);
  // 失能电机
  canmotor.Disable(can_id);
  running = false;
}

void Syntron_Servo::SetPositionMode()
{
  lock_guard<recursive_mutex> lock(can_mutex);
  paramLock(false);
  canmotor.SetControlMode(can_id, CanCode::POSITION_MODE);
  setPositionWindow();
  getPosition(position);
  getVelocity(velocity);
}

void Syntron_Servo::update_spin()
{
  while (update_loop_)
  {
    // 频率约为100HZ
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    // 若到位，则不必更新
    if (inplaceCheck())
    {
      inplace = true;
    }
    else
    {
      inplace = false;
      // 更新电机数据
      getPosition(position);
      getVelocity(velocity);
    }
  }
}

void Syntron_Servo::setPosition(float cm)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  if (running == false)
  {
    return;
  }
  // 限位检测
  if (cm >= highlimit)
  {
    printf("Target position is out of limit, motor is reaching UP LIMIT POSITION.\n");
    cm = highlimit;
  }
  else if (cm <= lowlimit)
  {
    printf("Target position is out of limit, motor is reaching DOWN LIMIT POSITION.\n");
    cm = lowlimit;
  }

  canmotor.SetTargetPosition(can_id, cm2pos(cm));
  // 设置为绝对位置触发，触发后电机方运行
  canmotor.PPAbsoluteTrigger(can_id);
}

bool Syntron_Servo::inplaceCheck()
{
  lock_guard<recursive_mutex> lock(can_mutex);
  if (canmotor.isMotorAtPosition(can_id))
  {
    return true;
  }
  return false;
}

void Syntron_Servo::setDegree(float degree)
{
  // 角度较小，没有限位
  lock_guard<recursive_mutex> lock(can_mutex);
  setPosition(deg2cm(degree));
}

float Syntron_Servo::getDegree(void)
{
  return cm2deg(position);
}


void Syntron_Servo::setVelocity(float linear_velocity)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  
  // 将直线速度(m/s)转换为rpm
  // m/s -> mm/min: 乘以1000转换m到mm，乘以60转换s到min
  float mm_per_min = linear_velocity * 1000.0 * 60.0;
  // mm/min -> rpm: 根据机械关系，6圈对应5mm，即1mm对应6/5圈
  float rpm = mm_per_min * 6.0 / 5.0;
  
  if ((rpm > SyntronCanCode::VELOCITY_MAX) || (rpm < SyntronCanCode::VELOCITY_MIN))
  {
    printf("Linear velocity is out of limit (converted to %.2f rpm), please check it and set again.\n", rpm);
    return;
  }
  enableWriteFlash(true);
  paramLock(false);
  canmotor.SetProfileVelocity(can_id, rpm * 10);
  enableWriteFlash(false);
}

void Syntron_Servo::setStop(void)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  // 失能后再使能电机
  canmotor.SDO_Write(can_id, CanCode::SDO_W2, CanDict::CONTROL_WORD, 0x00, 0x07);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  canmotor.SDO_Write(can_id, CanCode::SDO_W2, CanDict::CONTROL_WORD, 0x00, 0x0F);
}

void Syntron_Servo::setHighLimit(float cm)
{
  if (cm < lowlimit)
  {
    printf("Up limit cannot be lower than down limit.\n");
    return;
  }
  highlimit = cm;
}

void Syntron_Servo::setLowLimit(float cm)
{
  if (cm > highlimit)
  {
    printf("Down limit cannot be bigger than up limit.\n");
    return;
  }
  lowlimit = cm;
}

bool Syntron_Servo::getHighLimit(float& cm)
{
  cm = highlimit;
  return true;
}

bool Syntron_Servo::getLowLimit(float& cm)
{
  cm = lowlimit;
  return true;
}

bool Syntron_Servo::reversePositionCountDirection()
{
  lock_guard<recursive_mutex> lock(can_mutex);
  can_frame cf = canmotor.SDO_Write(can_id, CanCode::SDO_Read, SyntronCanCode::REVERSE_A_COUNT_DIRECTION, 0, 0);

  int16_t value = 0;
  if (cf.data[0] != 0)
  {
    // 低位优先组合四字节数据为 16 位整数
    for (int i = 0; i < 2; ++i)
    {
      value |= static_cast<int16_t>(cf.data[4 + i]) << (i * 8);
    }
    if (value == 0)
      value = 1;
    else
      value = 0;
    paramLock(false);
    enableWriteFlash(true);
    // 写入新值
    canmotor.SDO_Write(can_id, CanCode::SDO_W2, SyntronCanCode::REVERSE_A_COUNT_DIRECTION, 0x00, value);
    enableWriteFlash(false);
    return true;
  }
  return false;
}

void Syntron_Servo::enableWriteFlash(bool enable)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  canmotor.SDO_Write(can_id, CanCode::SDO_W2, SyntronCanCode::PARAM_WRITE_FLASH, 0x00, (uint16_t)enable);
}

void Syntron_Servo::paramLock(bool paramlock)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  if (paramlock)
    canmotor.SDO_Write(can_id, CanCode::SDO_W2, SyntronCanCode::PARAM_LOCK, 0x00, (uint16_t)8840);
  else
    canmotor.SDO_Write(can_id, CanCode::SDO_W2, SyntronCanCode::PARAM_LOCK, 0x00, (uint16_t)8847);
}

bool Syntron_Servo::checkAlarm(void)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  can_frame cf = canmotor.SDO_Write(can_id, CanCode::SDO_Read, SyntronCanCode::ALARM_CODE, 0x00, 0x00);

  // 异常即为错误，重启驱动器
  if (cf.data[0] == 0)
    return true;

  // 低位优先组合四字节数据为 32 位整数
  int16_t value = 0;
  for (int i = 0; i < 2; ++i)
  {
    value |= static_cast<int16_t>(cf.data[4 + i]) << (i * 8);
  }
  return value;
}

void Syntron_Servo::clearAlarm(void)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  canmotor.SDO_Write(can_id, CanCode::SDO_W2, SyntronCanCode::ALARM, 0x00, 0);
}

bool Syntron_Servo::getPosition(float& cm)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  int32_t pos;
  if (canmotor.GetPositionActualValue(can_id, pos))
  {
    cm = pos2cm(pos);

    return true;
  }
  else
  {
    printf("Position Message Receive Error.\n");
    return false;
  }
}

bool Syntron_Servo::getEncoderPosition(int32_t& pos)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  if (canmotor.GetPositionActualValue(can_id, pos))
  {
    return true;
  }
  else
  {
    printf("Position Message Receive Error.\n");
    return false;
  }
}

// 这里的速度单位为滑轨移动的m/s，与电机转速不同
bool Syntron_Servo::getVelocity(float& linear_velocity)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  int32_t vel;
  if (canmotor.GetVelocitySensorActualValue(can_id, vel))
  {
    // 单位为0.1rpm，先转换为rpm
    float rpm = vel / 10.0;
    // 转换为直线速度：6圈对应5mm，即1圈对应5/6 mm
    // 直线速度 = rpm * (5/6) mm/min
    float mm_per_min = rpm * 5.0 / 6.0;
    // 转换为m/s：mm/min -> m/s (除以1000转换mm到m，除以60转换min到s)
    linear_velocity = mm_per_min / 1000.0 / 60.0; // m/s
    return true;
  }
  else
  {
    printf("Velocity Message Receive Error.\n");
    return false;
  }
}

void Syntron_Servo::setPositionWindow(void)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  enableWriteFlash(true);
  // 按照推荐，均设为10
  canmotor.SDO_Write(can_id, CanCode::SDO_W4, 0x6067, 0x00, 100);
  canmotor.SDO_Write(can_id, CanCode::SDO_W2, 0x6068, 0x00, 100);
  enableWriteFlash(false);
}

// 将当前位置设置为零点
bool Syntron_Servo::setZeroPoint()
{
  lock_guard<recursive_mutex> lock(can_mutex);
  enableWriteFlash(true);
  paramLock(false);

  canmotor.SetControlMode(can_id, CanCode::HOMING_MODE);
  canmotor.SDO_Write(can_id, CanCode::SDO_W1, 0x6098, 0x00, (int8_t)35);
  canmotor.SDO_Write(can_id, CanCode::SDO_W4, 0x6099, 0x00, 0);

  canmotor.SDO_Write(can_id, CanCode::SDO_W2, CanDict::CONTROL_WORD, 0x00, 0x0F);
  canmotor.SDO_Write(can_id, CanCode::SDO_W2, CanDict::CONTROL_WORD, 0x00, 0x5F);
  canmotor.SDO_Write(can_id, CanCode::SDO_W2, CanDict::CONTROL_WORD, 0x00, 0x4F);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  setStop();
  canmotor.Disable(can_id);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  SetPositionMode();
  canmotor.Enable(can_id);
  float cm = 0.1;
  // 更新电机位置
  getPosition(cm);
  if (abs(cm) < 0.01)
    return true;
  else
    return false;
}

void Syntron_Servo::resetDriver()
{
  lock_guard<recursive_mutex> lock(can_mutex);
  clearAlarm();
  canmotor.SDO_Write(can_id, CanCode::SDO_W2, SyntronCanCode::RESET, 0x00, 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  canmotor.activate(can_id);
  SetPositionMode();
  canmotor.Enable(can_id);
}

float Syntron_Servo::pos2cm(int32_t pos)
{
  return pos / 10000.0 / 6 * 0.5;
}

int32_t Syntron_Servo::cm2pos(float cm)
{
  return cm * 10000.0 * 6 / 0.5;
}

float Syntron_Servo::pos2deg(int32_t pos)
{
  return pos / 10000.0 * 360;
}

float Syntron_Servo::cm2deg(float cm)
{
  return pos2deg(cm2pos(cm));
}
float Syntron_Servo::deg2cm(float deg)
{
  return pos2cm(deg2pos(deg));
}
int32_t Syntron_Servo::deg2pos(float deg)
{
  return deg / 360.0 * 10000.0;
}

}  // namespace rpp