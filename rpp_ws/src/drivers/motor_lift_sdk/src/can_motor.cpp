#include "motor_lift/can_motor.h"
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
namespace rpp
{

// 构造函数，初始化 CAN 通信
CanCommunication::CanCommunication(const std::string& interfaceName)
{
  sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);  // 创建原始 CAN 套接字
  if (sock < 0)
  {  // 如果套接字创建失败，输出错误并退出
    printf("Error while opening socket\n");
    exit(EXIT_FAILURE);
  }

  struct ifreq ifr;                                                        // 网络接口请求结构体
  strncpy(ifr.ifr_name, interfaceName.c_str(), sizeof(ifr.ifr_name) - 1);  // 设置接口名
  ioctl(sock, SIOCGIFINDEX, &ifr);                                         // 获取网络接口的索引

  struct sockaddr_can addr;            // CAN 地址结构体
  addr.can_family = AF_CAN;            // 设置地址族为 CAN
  addr.can_ifindex = ifr.ifr_ifindex;  // 设置接口索引

  if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
  {  // 绑定套接字到指定的 CAN 接口
    printf("Error while binding socket\n");
    exit(EXIT_FAILURE);
  }

  activeAllNodes();
}

// 析构函数，关闭 CAN 套接字
CanCommunication::~CanCommunication()
{
  if (sock >= 0)
  {
    close(sock);  // 关闭套接字
  }
}

// 发送 CAN 消息
void CanCommunication::sendCanMessage(const can_frame& frame)
{
  if (write(sock, &frame, sizeof(frame)) != sizeof(frame))
  {  // 写入套接字发送数据
    printf("Error while sending CAN message\n");
  }
}

// 接收 CAN 消息
int CanCommunication::receiveCanMessages(can_frame& cf)
{
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(sock, &readfds);
  // 超时时间最大为500ms
  int timeout_ms = 500;
  // 计算超时时间
  struct timeval timeout;
  timeout.tv_sec = timeout_ms / 1000;
  timeout.tv_usec = (timeout_ms % 1000) * 1000;

  // 使用 select 函数等待套接字可读
  int activity = select(sock + 1, &readfds, NULL, NULL, &timeout);
  if (activity == -1)
  {
    // 发生错误
    perror("select error\n");
    return -1;
  }
  else if (activity == 0)
  {
    // perror("Can message receive timeout.\n");
    // 超时
    return 0;
  }
  else
  {
    // 套接字可读，进行读取操作
    return read(sock, &cf, sizeof(cf));
  }
}

// 激活所有Can节点
void CanCommunication::activeAllNodes(void)
{
  can_frame cf;
  cf.can_id = 0x000;
  cf.can_dlc = 2;
  cf.data[0] = 0x01;
  cf.data[1] = 0x00;
  sendCanMessage(cf);
}

/*激活节点*/
void CanCommunication::activate(uint8_t CANopen_ID)
{
  can_frame cf;
  cf.can_id = 0x000;
  cf.can_dlc = 2;
  cf.data[0] = 0x01;
  cf.data[1] = CANopen_ID;
  sendCanMessage(cf);
}

can_frame CanCommunication::SDO_Write(uint8_t CANopen_ID, uint8_t CMD, uint16_t Index, uint8_t SubIndex, uint32_t DATA)
{
  can_frame cf, recv_cf;

  cf.can_id = 0x600 + CANopen_ID;
  cf.can_dlc = 8;
  cf.data[0] = CMD;
  cf.data[1] = (uint8_t)(Index & 0xFF);
  cf.data[2] = (uint8_t)(Index >> 8 & 0xFF);
  cf.data[3] = SubIndex;
  cf.data[4] = (uint8_t)(DATA & 0xFF);
  cf.data[5] = (uint8_t)(DATA >> 8 & 0xFF);
  cf.data[6] = (uint8_t)(DATA >> 16 & 0xFF);
  cf.data[7] = (uint8_t)(DATA >> 24 & 0xFF);

  sendCanMessage(cf);
  // 可能会有错位的情况，多读取几次
  for (int i = 0; i < 5; ++i)
  {
    if (SDO_Recv(CANopen_ID, recv_cf))
    {
      // 检验接收值
      uint16_t cmd = 0;
      for (int i = 0; i < 2; ++i)
      {
        cmd |= static_cast<uint16_t>(recv_cf.data[1 + i]) << (i * 8);
      }
      if (cmd == Index)
      {
        // 接受成功，返回正确数据
        return recv_cf;
      }
      else
      {
        printf("Can SDO Write Error: Index check error, Correct Index:0x%x, Received Index:0x%x\n", Index, cmd);
      }
    }
  }
  // data0位为0，表示错误
  recv_cf.data[0] = 0;
  printf("Can SDO Write Error: Index receive error, Index:0x%x\n", Index);
  return recv_cf;
}

void CanCommunication::parse_error_code(can_frame cf)
{
  uint16_t error = 0;
  for (int i = 0; i < 2; ++i)
  {
    // 低位在前
    error |= static_cast<uint16_t>(cf.data[i]) << (i * 8);
  }
  printf("Error Code: 0x%04x \n", error);
}

bool CanCommunication::SDO_Recv(uint8_t CANopen_ID, can_frame& cf)
{
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
      printf("SDO_Recv timeout after %d ms\n", timeout_ms);
      return false;
    }

    int nbytes = receiveCanMessages(cf);
    // 接收到本设备数据包，返回
    if ((nbytes > 0) && (cf.can_id == 0x580 + CANopen_ID))
    {
      return true;
    }
    // 接收到错误码，进行处理
    if ((nbytes > 0) && (cf.can_id == 0x080 + CANopen_ID))
    {
      printf("ERROR Code Received.\n");
      parse_error_code(cf);
      return false;
    }
    // 未接受到本设备，继续等待
  }
}

void CanMotor::parse_error_code(can_frame cf)
{
  uint16_t error = 0;
  for (int i = 0; i < 2; ++i)
  {
    // 低位在前
    error |= static_cast<uint16_t>(cf.data[i]) << (i * 8);
  }
  // 根据错误码输出对应的错误信息
  switch (error)
  {
    case 0x0001:
      printf("Error Code: 0x%04x, Error Message: Encoder fault ABZ alarm EC\n", error);
      break;
    case 0x0002:
      printf("Error Code: 0x%04x, Error Message: Encoder fault UVW alarm EH\n", error);
      break;
    case 0x0003:
      printf("Error Code: 0x%04x, Error Message: Position tolerance exceeded PE\n", error);
      break;
    case 0x0004:
      printf("Error Code: 0x%04x, Error Message: Stall ST\n", error);
      break;
    case 0x0005:
      printf("Error Code: 0x%04x, Error Message: Current sampling (midpoint) fault IC\n", error);
      break;
    case 0x0006:
      printf("Error Code: 0x%04x, Error Message: Overload OL\n", error);
      break;
    case 0x0007:
      printf("Error Code: 0x%04x, Error Message: Undervoltage LU\n", error);
      break;
    case 0x0008:
      printf("Error Code: 0x%04x, Error Message: Overvoltage OU\n", error);
      break;
    case 0x0009:
      printf("Error Code: 0x%04x, Error Message: Overcurrent OC\n", error);
      break;
    case 0x000A:
      printf("Error Code: 0x%04x, Error Message: Discharge alarm, instantaneous power too large LH\n", error);
      break;
    case 0x000B:
      printf("Error Code: 0x%04x, Error Message: Discharge circuit frequent action, average power too large OH\n",
             error);
      break;
    case 0x000C:
      printf("Error Code: 0x%04x, Error Message: Parameter read/write exception PA\n", error);
      break;
    case 0x000D:
      printf("Error Code: 0x%04x, Error Message: Input port function definition repeated ID\n", error);
      break;
    case 0x000E:
      printf("Error Code: 0x%04x, Error Message: Communication watchdog triggered CL\n", error);
      break;
    case 0x000F:
      printf("Error Code: 0x%04x, Error Message: Motor over - temperature alarm OT\n", error);
      break;
    default:
      printf("Error Code: 0x%04x, Error Message: Unknown error\n", error);
      break;
  }
}

void CanMotor::SetControlMode(uint8_t CANopen_ID, uint8_t CANopen_mode)
{
  SDO_Write(CANopen_ID, CanCode::SDO_W1, CanDict::MODES_OF_OPERATION, 0x00, CANopen_mode);
}

void CanMotor::Enable(uint8_t CANopen_ID)
{
  // 使能写0x6040分别为6、7、F
  SDO_Write(CANopen_ID, CanCode::SDO_W2, CanDict::CONTROL_WORD, 0x00, 0x06);

  SDO_Write(CANopen_ID, CanCode::SDO_W2, CanDict::CONTROL_WORD, 0x00, 0x07);

  SDO_Write(CANopen_ID, CanCode::SDO_W2, CanDict::CONTROL_WORD, 0x00, 0x0F);
}

void CanMotor::Disable(uint8_t CANopen_ID)
{
  // SDO_Write(CANopen_ID, CanCode::SDO_W2, CanDict::CONTROL_WORD, 0x00, 0x06);
  // 失能写0x6040为7
  SDO_Write(CANopen_ID, CanCode::SDO_W2, CanDict::CONTROL_WORD, 0x00, 0x07);
}

void CanMotor::SetTargetPosition(uint8_t CANopen_ID, int32_t pos)
{
  SDO_Write(CANopen_ID, CanCode::SDO_W4, CanDict::TARGET_POSITION, 0x00, pos);
}

void CanMotor::SetProfileVelocity(uint8_t CANopen_ID, uint32_t vel)
{
  SDO_Write(CANopen_ID, CanCode::SDO_W4, CanDict::PROFILE_VELOCITY, 0x00, vel);
}

void CanMotor::PPIncrementalTrigger(uint8_t CANopen_ID)
{
  // STEP1:6040 bits4清0
  SDO_Write(CANopen_ID, CanCode::SDO_W4, CanDict::CONTROL_WORD, 0x00, 0x0F);

  // STEP2:置bits4为1，电机运动
  SDO_Write(CANopen_ID, CanCode::SDO_W4, CanDict::CONTROL_WORD, 0x00, 0x5F);

  // STEP3:6040 bits4清0
  SDO_Write(CANopen_ID, CanCode::SDO_W4, CanDict::CONTROL_WORD, 0x00, 0x4F);
}

void CanMotor::PPAbsoluteTrigger(uint8_t CANopen_ID)
{
  SDO_Write(CANopen_ID, CanCode::SDO_W4, CanDict::CONTROL_WORD, 0x00, 0x0F);
  SDO_Write(CANopen_ID, CanCode::SDO_W4, CanDict::CONTROL_WORD, 0x00, 0x1F);
  SDO_Write(CANopen_ID, CanCode::SDO_W4, CanDict::CONTROL_WORD, 0x00, 0x0F);
}

bool CanMotor::GetPositionActualValue(uint8_t CANopen_ID, int32_t& pos)
{
  can_frame cf = SDO_Write(CANopen_ID, CanCode::SDO_Read, CanDict::POSITION_ACTUAL_VALUE, 0x00, 0x00);

  if (cf.data[0] == 0)
    return false;

  // 低位优先组合四字节数据为 32 位整数
  pos = 0;
  for (int i = 0; i < 4; ++i)
  {
    pos |= static_cast<int32_t>(cf.data[4 + i]) << (i * 8);
  }
  return true;
}

bool CanMotor::GetVelocityActualValue(uint8_t CANopen_ID, int32_t& vel)
{
  can_frame cf = SDO_Write(CANopen_ID, CanCode::SDO_Read, CanDict::VELOCITY_ACTUAL_VALUE, 0x00, 0x00);

  if (cf.data[0] == 0)
    return false;

  // 低位优先组合四字节数据为 32 位整数
  vel = 0;
  for (int i = 0; i < 4; ++i)
  {
    vel |= static_cast<int32_t>(cf.data[4 + i]) << (i * 8);
  }
  return true;
}
bool CanMotor::GetVelocitySensorActualValue(uint8_t CANopen_ID, int32_t& vel)
{
  can_frame cf = SDO_Write(CANopen_ID, CanCode::SDO_Read, CanDict::VELOCITY_SENSOR_ACTUAL_VALUE, 0x00, 0x00);

  if (cf.data[0] == 0)
    return false;

  // 低位优先组合四字节数据为 32 位整数
  vel = 0;
  for (int i = 0; i < 4; ++i)
  {
    vel |= static_cast<int32_t>(cf.data[4 + i]) << (i * 8);
  }
  return true;
}

bool CanMotor::GetProfileVelocity(uint8_t CANopen_ID, uint32_t& vel)
{
  can_frame cf = SDO_Write(CANopen_ID, CanCode::SDO_Read, CanDict::PROFILE_VELOCITY, 0x00, 0x00);

  if (cf.data[0] == 0)
    return false;

  // 低位优先组合四字节数据为 32 位整数
  vel = 0;
  for (int i = 0; i < 4; ++i)
  {
    vel |= static_cast<uint32_t>(cf.data[4 + i]) << (i * 8);
  }
  return true;
}

bool CanCommunication::checkSDORecv(can_frame cf, uint16_t Index)
{
  // 检验接收值
  uint16_t cmd = 0;
  for (int i = 0; i < 2; ++i)
  {
    cmd |= static_cast<uint16_t>(cf.data[1 + i]) << (i * 8);
  }
  if (cmd != Index)
  {
    printf("SDO Message Receive Check Error.\n");
    return false;
  }
  return true;
}

// 检测电机是否到位
bool CanMotor::isMotorAtPosition(uint8_t CANopen_ID)
{
  // 发送读取状态字 6041h 的请求
  can_frame cf = SDO_Write(CANopen_ID, CanCode::SDO_Read, CanDict::STATUS_WORD, 0x00, 0x00);

  if (cf.data[0] == 0)
    return false;

  uint16_t status = 0;
  for (int i = 0; i < 2; ++i)
  {
    status |= static_cast<uint16_t>(cf.data[4 + i]) << (i * 8);
  }
  // 检查 bit10 的值
  return (status & (1 << 10)) != 0;
}

// 急停置位，设置 6040h 的 bit2 为 1
void CanMotor::setEmergencyStop(uint8_t CANopen_ID)
{
  can_frame cf = SDO_Write(CANopen_ID, CanCode::SDO_Read, CanDict::CONTROL_WORD, 0x00, 0x00);
  if (cf.data[0] != 0)
  {
    // 先读取当前状态字
    uint16_t status_word = 0;
    // 低位优先组合两字节数据为 16 位整数
    status_word = static_cast<uint16_t>(cf.data[4]) | (static_cast<int16_t>(cf.data[5]) << 8);
    // 设置 bit2 为 1
    status_word |= (1 << 2);
    // 将新的状态字写回
    SDO_Write(CANopen_ID, CanCode::SDO_W4, CanDict::CONTROL_WORD, 0x00, static_cast<uint32_t>(status_word));
  }
  else
  {
    printf("Failed to get status word 6040h.\n");
  }
}

// 急停复位，设置 6040h 的 bit2 为 0
void CanMotor::resetEmergencyStop(uint8_t CANopen_ID)
{
  can_frame cf = SDO_Write(CANopen_ID, CanCode::SDO_Read, CanDict::CONTROL_WORD, 0x00, 0x00);
  if (cf.data[0] != 0)
  {
    // 先读取当前状态字
    uint16_t status_word = 0;
    // 低位优先组合两字节数据为 16 位整数
    status_word = static_cast<uint16_t>(cf.data[4]) | (static_cast<int16_t>(cf.data[5]) << 8);
    // 设置 bit2 为 0
    status_word &= ~(1 << 2);
    // 将新的状态字写回
    SDO_Write(CANopen_ID, CanCode::SDO_W4, CanDict::CONTROL_WORD, 0x00, static_cast<uint32_t>(status_word));
  }
  else
  {
    printf("Failed to get status word 6040h.\n");
  }
}

void CanMotor::setHomeOffset(uint8_t CANopen_ID, int32_t pos)
{
  SDO_Write(CANopen_ID, CanCode::SDO_W4, CanDict::HOME_OFFSET, 0x00, pos);
}

bool CanMotor::getHomeOffset(uint8_t CANopen_ID, int32_t& pos)
{
  // 发送读取 0x607C 地址的请求
  can_frame cf = SDO_Write(CANopen_ID, CanCode::SDO_Read, CanDict::HOME_OFFSET, 0x00, 0x00);

  // 接收 SDO 响应
  if (cf.data[0] == 0)
    return false;

  // 低位优先组合四字节数据为 32 位整数
  pos = 0;
  for (int i = 0; i < 4; ++i)
  {
    pos |= static_cast<int32_t>(cf.data[4 + i]) << (i * 8);
  }
  return true;
}

void CanMotor::setHomingMethod(uint8_t CANopen_ID, int8_t hm)
{
  SDO_Write(CANopen_ID, CanCode::SDO_W1, CanDict::HOMING_METHOD, 0x00, hm);
}

void CanMotor::setLowSoftPositionLimit(uint8_t CANopen_ID, int32_t pos)
{
  SDO_Write(CANopen_ID, CanCode::SDO_W4, CanDict::SOFT_POSITION_LIMIT, 0x01, pos);
}

void CanMotor::setHighSoftPositionLimit(uint8_t CANopen_ID, int32_t pos)
{
  SDO_Write(CANopen_ID, CanCode::SDO_W4, CanDict::SOFT_POSITION_LIMIT, 0x02, pos);
}

bool CanMotor::getLowSoftPositionLimit(uint8_t CANopen_ID, int32_t& pos)
{
  // 发送读取低位软限位的请求
  can_frame cf = SDO_Write(CANopen_ID, CanCode::SDO_Read, CanDict::SOFT_POSITION_LIMIT, 0x01, 0x00);

  // 接收 SDO 响应
  if (cf.data[0] == 0)
    return false;

  // 低位优先组合四字节数据为 32 位整数
  pos = 0;
  for (int i = 0; i < 4; ++i)
  {
    pos |= static_cast<int32_t>(cf.data[4 + i]) << (i * 8);
  }
  return true;
}

bool CanMotor::getHighSoftPositionLimit(uint8_t CANopen_ID, int32_t& pos)
{
  // 发送读取高位软限位的请求
  can_frame cf = SDO_Write(CANopen_ID, CanCode::SDO_Read, CanDict::SOFT_POSITION_LIMIT, 0x02, 0x00);

  if (cf.data[0] == 0)
    return false;

  // 低位优先组合四字节数据为 32 位整数
  pos = 0;
  for (int i = 0; i < 4; ++i)
  {
    pos |= static_cast<int32_t>(cf.data[4 + i]) << (i * 8);
  }
  return true;
}

}  // namespace rpp