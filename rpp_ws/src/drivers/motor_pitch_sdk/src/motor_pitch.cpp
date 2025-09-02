#include "motor_pitch/motor_pitch.h"

using namespace std;

namespace rpp
{

Ti5CRA_Servo::Ti5CRA_Servo(string canport, char canid) : canComm(canport)
{
  // 设置 CAN ID
  can_id = canid;
  sendFrame.can_id = canid;
  update_loop_ = true;
  inplace = true;
  update_thread_ = nullptr;
  running = false;
  param_init();
}

Ti5CRA_Servo::~Ti5CRA_Servo()
{
  setStop();
  if (update_thread_ != nullptr)
  {
    update_loop_ = false;
    update_thread_->join();
    delete update_thread_;
    update_thread_ = nullptr;
  }
}

void Ti5CRA_Servo::start()
{
  running = true;
  if (update_thread_ == nullptr)
    update_thread_ = new thread(&Ti5CRA_Servo::update_spin, this);
}

void Ti5CRA_Servo::stop()
{
  setStop();
  running = false;
}

void Ti5CRA_Servo::param_init()
{
  // 初始化电机数据
  if (!getPosition(position))
    printf("Failed to initialize motor's POSITION.\n");
  if (!getHighLimit(highlimit))
    printf("Failed to initialize motor's UP POSITION LIMIT.\n");
  if (!getLowLimit(lowlimit))
    printf("Failed to initialize motor's Down POSITION LIMIT.\n");
  setVelocity(2);
}

void Ti5CRA_Servo::update_spin()
{
  while (update_loop_)
  {
    // 频率约为100HZ
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    // 若到位，则不必更新
    if (inplace == true)
      continue;
    else
    {
      // 更新电机数据
      getPosition(position);
      getVelocity(velocity);
      //检测是否到位
      if ((abs(position - target_position) < 0.1) && (velocity == 0))
      {
        inplace = true;
      }
    }
  }
}

void Ti5CRA_Servo::setCurrentAccordingToPosition(float position)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  // 根据电机位置，设置四档启动电流
  if (0 <= position && position <= 20)
    setCurrentBeforeMove(-1);
  if (20 < position && position <= 40)
    setCurrentBeforeMove(-2);
  if (40 < position && position <= 70)
    setCurrentBeforeMove(-4);
  if (70 < position && position <= 90)
    setCurrentBeforeMove(-5);
}

void Ti5CRA_Servo::setPosition(float deg)
{
  lock_guard<recursive_mutex> lock(can_mutex);

  if (!running)
    return;

  // 限位检测
  if (deg >= highlimit)
  {
    printf("Motor is reaching UP LIMIT POSITION.\n");
    deg = highlimit;
  }
  else if (deg <= lowlimit)
  {
    printf("Motor is reaching DOWN LIMIT POSITION.\n");
    deg = lowlimit;
  }

  // 若当前位置与目标位置相同，则不发送命令
  if (deg == position)
  {
    target_position = deg;
    inplace = true;
    return;
  }

  // 若当前位置与目标位置不同，则发送命令
  // 首先根据当前位置设置启动电流
  setCurrentAccordingToPosition(position);
  int pos = deg2pos(deg);
  set_i32data(Ti5CRAFunctionCode::SET_POSITION, pos);
  target_position = deg;
  inplace = false;
}

void Ti5CRA_Servo::setVelocity(float rpm)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  if ((rpm > Ti5CRAFunctionCode::VELOCITY_MAX) || (rpm < Ti5CRAFunctionCode::VELOCITY_MIN))
  {
    printf("Velocity is out of limit, please check it and set again.\n");
    return;
  }
  setUpVelLimit(rpm);
  setDownVelLimit(-rpm);
}

void Ti5CRA_Servo::setStop(int32_t t_ms)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  set_i32data(Ti5CRAFunctionCode::SET_STOP, t_ms);
}

bool Ti5CRA_Servo::getPosition(float& deg)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  int32_t pos;
  if (get_i32data(Ti5CRAFunctionCode::GET_POSITION, pos))
  {
    deg = pos2deg(pos);
    return true;
  }
  else
  {
    printf("Position Message Receive Error.\n");
    return false;
  }
}

bool Ti5CRA_Servo::getEncoderPosition(int32_t& pos)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  if (get_i32data(Ti5CRAFunctionCode::GET_ENCODER_POSITION, pos))
  {
    return true;
  }
  else
  {
    printf("encoder_position Message Receive Error.\n");
    return false;
  }
}

bool Ti5CRA_Servo::getVelocity(float& rpm)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  int32_t hz;
  if (get_i32data(Ti5CRAFunctionCode::GET_VELOCITY, hz))
  {
    rpm = hz2rpm(hz);
    return true;
  }
  else
  {
    printf("Velocity Message Receive Error.\n");
    return false;
  }
}

void Ti5CRA_Servo::setPositionOffect(int32_t pos)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  set_i32data(Ti5CRAFunctionCode::SET_POSITION_OFFSET, pos);
}

bool Ti5CRA_Servo::getPositionOffset(int32_t& pos)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  if (get_i32data(Ti5CRAFunctionCode::GET_POSITION_OFFSET, pos))
  {
    return true;
  }
  else
  {
    printf("PositionOffect Message Receive Error.\n");
    return false;
  }
}

// 将当前位置设置为零点
bool Ti5CRA_Servo::setZeroPoint()
{
  lock_guard<recursive_mutex> lock(can_mutex);
  int32_t current_pos, current_posofs, ofs;
  if (getEncoderPosition(current_pos))
  {
    if (getPositionOffset(current_posofs))
    {
      ofs = current_pos + current_posofs;
      setPositionOffect(ofs);
      // 将数据写入Flash，否则掉电不会保存
      writeFlash();
      // 更新电机位置
      getPosition(position);
      return true;
    }
  }
  return false;
}

void Ti5CRA_Servo::setCurrentBeforeMove(float current)
{
  // 若当前设置的电流与目标电流相同，则不发送命令
  if (current == current_before_move)
    return;
  lock_guard<recursive_mutex> lock(can_mutex);
  if ((current > 20) || (current < -20))
  {
    printf("Current is out of limit, please check it and set again.\n");
    return;
  }
  set_i32data(Ti5CRAFunctionCode::SET_CURRENT_BEFORE_MOVE, current * 1000);
  current_before_move = current;
}

void Ti5CRA_Servo::moveForward(float deg)
{
  float pos = position + deg;
  setPosition(pos);
}

void Ti5CRA_Servo::moveBackward(float deg)
{
  float pos = position - deg;
  setPosition(pos);
}

bool Ti5CRA_Servo::getHighLimit(float& deg)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  int32_t pos;
  if (get_i32data(Ti5CRAFunctionCode::GET_UPLIMIT, pos))
  {
    deg = pos2deg(pos);
    return true;
  }
  else
  {
    printf("PositionOffect Message Receive Error.\n");
    return false;
  }
}

bool Ti5CRA_Servo::getLowLimit(float& deg)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  int32_t pos;
  if (get_i32data(Ti5CRAFunctionCode::GET_DOWNLIMIT, pos))
  {
    deg = pos2deg(pos);
    return true;
  }
  else
  {
    printf("PositionOffect Message Receive Error.\n");
    return false;
  }
}

// 上下限位为相对零点的位置，不是绝对位置
// 零点改变后，上下限位亦改变
bool Ti5CRA_Servo::setHighLimit(float deg)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  float low_deg;
  if (getLowLimit(low_deg))
  {
    // 若上限位小于下限位，则拒绝设置
    if (low_deg < deg)
    {
      int32_t pos = deg2pos(deg);
      set_i32data(Ti5CRAFunctionCode::SET_UPLIMIT, pos);
      // 将数据写入Flash，否则掉电不会保存
      writeFlash();
      highlimit = deg;
      return true;
    }
    else
      printf("The given UpLimit is SMALLER than current DownLimit.\n");
  }
  return false;
}

bool Ti5CRA_Servo::setLowLimit(float deg)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  float high_deg;
  if (getHighLimit(high_deg))
  {
    // 若下限位大于上限位，则拒绝设置
    if (high_deg > deg)
    {
      int32_t pos = deg2pos(deg);
      set_i32data(Ti5CRAFunctionCode::SET_DOWNLIMIT, pos);
      // 将数据写入Flash，否则掉电不会保存
      writeFlash();
      lowlimit = deg;
      return true;
    }
    else
      printf("The given DownLimit is BIGGER than current UpLimit.\n");
  }
  return false;
}

bool Ti5CRA_Servo::setUpVelLimit(float rpm)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  if (rpm > 0)
  {
    int32_t hz = rpm2hz(rpm);
    set_i32data(Ti5CRAFunctionCode::SET_UPVELLIMIT, hz);
    // 将数据写入Flash，否则掉电不会保存
    writeFlash();
    return true;
  }
  else
  {
    printf("Up Limit Velocity should BIGGER than 0.\n");
    return false;
  }
}

bool Ti5CRA_Servo::setDownVelLimit(float rpm)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  if (rpm < 0)
  {
    int32_t hz = rpm2hz(rpm);
    set_i32data(Ti5CRAFunctionCode::SET_DOWNVELLIMIT, hz);
    // 将数据写入Flash，否则掉电不会保存
    writeFlash();
    return true;
  }
  else
  {
    printf("Down Limit Velocity should SMALLER than 0.\n");
    return false;
  }
}

bool Ti5CRA_Servo::getUpVelLimit(float& rpm)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  int32_t hz;
  if (get_i32data(Ti5CRAFunctionCode::GET_UPVELLIMIT, hz))
  {
    rpm = hz2rpm(hz);
    return true;
  }
  else
  {
    printf("UpVelocityLimit Message Receive Error.\n");
    return false;
  }
}

bool Ti5CRA_Servo::getDownVelLimit(float& rpm)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  int32_t hz;
  if (get_i32data(Ti5CRAFunctionCode::GET_DOWNVELLIMIT, hz))
  {
    rpm = hz2rpm(hz);
    return true;
  }
  else
  {
    printf("DownVelocityLimit Message Receive Error.\n");
    return false;
  }
}

// 将当前参数写入到Flash
void Ti5CRA_Servo::writeFlash(void)
{
  lock_guard<recursive_mutex> lock(can_mutex);
  float rpm;
  if (getVelocity(rpm))
  {
    // 仅可在电机停止时写入Flash
    if (rpm == 0)
    {
      send_byteCmd(Ti5CRAFunctionCode::WRITE_FLASH);
      return;
    }
  }
  printf("Write flash failed, please ensure motor is stop.\n");
  return;
}

void Ti5CRA_Servo::creat_int32t_msg(can_frame& cf, uint8_t begin_pos, int32_t num)
{
  // 使用位运算低位优先依次取字节
  for (int i = 0; i < 4; ++i)
  {
    cf.data[i + begin_pos] = static_cast<unsigned char>((num >> (i * 8)) & 0xFF);
  }
}

int32_t Ti5CRA_Servo::parse_int32t_msg(can_frame& cf, uint8_t begin_pos)
{
  // 低位优先组合四字节数据为 32 位整数
  int32_t intValue = 0;
  for (int i = 0; i < 4; ++i)
  {
    intValue |= static_cast<int32_t>(cf.data[begin_pos + i]) << (i * 8);
  }
  return intValue;
}

void Ti5CRA_Servo::parse_error_code(can_frame& cf, uint8_t begin_pos)
{
  // Combine four bytes of data into a 32-bit integer in little-endian order
  int32_t intValue = 0;
  for (int i = 0; i < 4; ++i)
  {
    intValue |= static_cast<int32_t>(cf.data[begin_pos + i]) << (i * 8);
  }

  bool hasError = false;
  if (intValue & (1 << 0))
  {
    std::cout << "Internal software error in the motor, such as writing to FLASH while the motor is running."
              << std::endl;
    hasError = true;
  }
  if (intValue & (1 << 1))
  {
    std::cout << "Overvoltage." << std::endl;
    hasError = true;
  }
  if (intValue & (1 << 2))
  {
    std::cout << "Undervoltage, voltage is too low." << std::endl;
    hasError = true;
  }
  if (intValue & (1 << 4))
  {
    std::cout << "Startup error." << std::endl;
    hasError = true;
  }
  if (intValue & (1 << 5))
  {
    std::cout << "Speed feedback error." << std::endl;
    hasError = true;
  }
  if (intValue & (1 << 6))
  {
    std::cout << "Overcurrent." << std::endl;
    hasError = true;
  }
  if (intValue & (1 << 7))
  {
    std::cout << "Software error, such as writing to FLASH while the motor is running." << std::endl;
    hasError = true;
  }
  if (intValue & (1 << 16))
  {
    std::cout << "Encoder communication error." << std::endl;
    hasError = true;
  }
  if (intValue & (1 << 17))
  {
    std::cout << "Motor overheating." << std::endl;
    hasError = true;
  }
  if (intValue & (1 << 18))
  {
    std::cout << "Circuit board overheating." << std::endl;
    hasError = true;
  }
  if (intValue & (1 << 19))
  {
    std::cout << "Driver chip error." << std::endl;
    hasError = true;
  }
  if (!hasError)
  {
    std::cout << "Motor has no ERROR." << std::endl;
  }
}

bool Ti5CRA_Servo::get_i32data(char cmd, int32_t& num)
{
  sendFrame.can_dlc = 1;
  sendFrame.data[0] = cmd;
  canComm.sendCanMessage(sendFrame);
  // 接收CAN数据
  can_frame recv_cf;  // 定义一个 CAN 帧

  // 定义超时时间，单位为毫秒
  const int timeout_ms = 2000;
  // 记录开始时间
  auto start_time = std::chrono::steady_clock::now();

  // 过滤其他设备发送的数据
  while (1)
  {
    // 检查是否超时
    auto current_time = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
    if (elapsed_time >= timeout_ms)
    {
      // 超时处理
      printf("Can receive message timeout after %d ms\n", timeout_ms);
      return false;
    }

    int nbytes = canComm.receiveCanMessages(recv_cf);
    // 接收到本设备数据包，进行处理
    if ((nbytes > 0) && (recv_cf.can_id == can_id))
    {
      // 是否接收到需要的数据包
      if (recv_cf.data[0] == cmd)
      {
        num = parse_int32t_msg(recv_cf, 1);
        return true;
      }
      else
      {
        // 接收到了报错数据
        if (recv_cf.data[0] == Ti5CRAFunctionCode::ERROR)
        {
          cout << "Motor ERROR Detected, Please Check: " << endl;
          parse_error_code(recv_cf, 1);
          return false;
        }
      }
      // 未接受到所需数据，继续等待
    }
  }
}

void Ti5CRA_Servo::set_i32data(char cmd, int32_t num)
{
  sendFrame.can_dlc = 5;
  sendFrame.data[0] = cmd;
  creat_int32t_msg(sendFrame, 1, num);
  canComm.sendCanMessage(sendFrame);
}

void Ti5CRA_Servo::send_byteCmd(char cmd)
{
  sendFrame.can_dlc = 1;
  sendFrame.data[0] = cmd;
  canComm.sendCanMessage(sendFrame);
}

// 第一版，多圈版本
float Ti5CRA_Servo::pos2deg(int32_t pos)
{
  return float(pos) / 65536 / 101 * 360;
}

int32_t Ti5CRA_Servo::deg2pos(float deg)
{
  return (deg / 360) * 101 * 65536;
}

int32_t Ti5CRA_Servo::rpm2hz(float rpm)
{
  return rpm * 6 * 101 * 100 / 360;
}

float Ti5CRA_Servo::hz2rpm(int32_t hz)
{
  return float(hz) / 100 / 101 * 60;
}
}  // namespace rpp
