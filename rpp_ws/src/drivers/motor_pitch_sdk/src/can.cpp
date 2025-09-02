#include "motor_pitch/can.h"

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

}  // namespace rpp