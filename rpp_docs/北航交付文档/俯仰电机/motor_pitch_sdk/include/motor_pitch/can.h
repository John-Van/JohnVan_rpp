#ifndef CAN_H
#define CAN_H

#include <iostream>        // 包含输入输出流，用于打印日志或调试信息
#include <string>          // 包含 string 类的定义，用于字符串操作
#include <cstring>         // 包含 C 风格字符串操作函数的定义
#include <unistd.h>        // 包含 POSIX 系统调用，例如 close, read, write
#include <net/if.h>        // 包含网络接口相关的定义
#include <sys/ioctl.h>     // 包含 I/O 控制相关的函数定义，例如 SIOCGIFINDEX
#include <fcntl.h>         // 包含文件控制相关的定义
#include <linux/can.h>     // 包含 CAN 协议相关的定义
#include <linux/can/raw.h> // 包含原始 CAN 套接字定义
#include <sys/socket.h>    // 包含 socket 套接字的相关定义
#include <mutex>           // 包含互斥量的定义，用于线程同步

namespace rpp
{

  // CAN 通信类，用于发送和接收 CAN 消息
  class CanCommunication
  {
  public:
    // 构造函数，初始化 CAN 通信
    CanCommunication(const std::string &interfaceName);
    // 析构函数，关闭 CAN 套接字
    ~CanCommunication();
    // 发送 CAN 消息
    void sendCanMessage(const can_frame &frame);
    // 接收 CAN 消息
    int receiveCanMessages(can_frame &cf);

  private:
    int sock; // 套接字描述符
  };

} // namespace rpp

#endif