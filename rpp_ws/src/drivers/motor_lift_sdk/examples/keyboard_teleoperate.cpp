#include <stdio.h>
#include <unistd.h>
#include <termio.h>
#include <iostream>
#include <iomanip>
#include <limits>
#include "motor_lift/motor_lift.h"

using namespace rpp;
using namespace std;

int scanKeyboard()
{
  int in;
  struct termios new_settings;
  struct termios stored_settings;

  // 获取当前标准输入的终端属性
  if (tcgetattr(0, &stored_settings) == -1)
  {
    std::cerr << "Failed to get terminal attributes: " << std::strerror(errno) << std::endl;
    return -1;
  }

  new_settings = stored_settings;
  // 进入非规范模式，无需按回车键即可读取输入
  new_settings.c_lflag &= (~ICANON);
  // 不进行超时等待
  new_settings.c_cc[VTIME] = 0;
  // 至少读取一个字符
  new_settings.c_cc[VMIN] = 1;

  // 立即应用新的终端属性
  if (tcsetattr(0, TCSANOW, &new_settings) == -1)
  {
    std::cerr << "Failed to set terminal attributes: " << std::strerror(errno) << std::endl;
    return -1;
  }

  // 读取一个字符
  in = getchar();

  // 恢复原始的终端属性
  if (tcsetattr(0, TCSANOW, &stored_settings) == -1)
  {
    std::cerr << "Failed to restore terminal attributes: " << std::strerror(errno) << std::endl;
  }

  return in;
}

void showmenu(void)
{
  cout << "Motor Test" << endl;
  cout << "P: Set motor position." << endl;
  cout << "W: Motor move forward." << endl;
  cout << "S: Motor move backward." << endl;
  cout << "B: Motor stop." << endl;
  cout << "O: Set motor zero point." << endl;
  cout << "L: Set motor limits." << endl;
  cout << "I: Show motor info." << endl;
  cout << "Q: Program exit." << endl;
  cout << "M: Enable motor." << endl;
  cout << "N: Disable motor." << endl;
  cout << "R: Reset driver." << endl;
  cout << "H: Menu." << endl;
}

int main()
{
  Syntron_Servo syntron_servo("can0", 0x02);
  syntron_servo.start();
  cout << boolalpha << fixed << setprecision(2);
  showmenu();

  bool r;

  while (1)
  {
    int ascii = scanKeyboard();
    if (ascii == -1)
    {
      continue;
    }
    if (ascii == 'p' || ascii == 'P')
    {
      // 使用 std::cout 提示用户输入目标位置
      std::cout << "Please input target position.\n";
      float pos;
      // 循环验证用户输入，直到输入有效数字
      while (!(std::cin >> pos))
      {
        std::cin.clear();                                                    // 清除错误标志
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // 忽略无效输入
        std::cout << "Invalid input. Please enter a valid number: ";
      }
      // 清空输入缓冲区
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      syntron_servo.setPosition(pos);
      cout << "Set position: " << pos << endl;
    }
    if (ascii == 'l' || ascii == 'L')
    {
      float highlim, lowlim, vellim;

      // 使用 std::cout 提示用户输入高限位位置
      std::cout << "Please input HIGH LIMIT position.\n";
      // 循环验证用户输入，直到输入有效数字
      while (!(std::cin >> highlim))
      {
        std::cin.clear();                                                    // 清除错误标志
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // 忽略无效输入
        std::cout << "Invalid input. Please enter a valid number: ";
      }
      // 清空输入缓冲区
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      syntron_servo.setHighLimit(highlim);
      cout << "Set HIGH LIMIT Position: " << highlim << endl;

      // 使用 std::cout 提示用户输入低限位位置
      std::cout << "Please input LOW LIMIT position.\n";
      // 循环验证用户输入，直到输入有效数字
      while (!(std::cin >> lowlim))
      {
        std::cin.clear();                                                    // 清除错误标志
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // 忽略无效输入
        std::cout << "Invalid input. Please enter a valid number: ";
      }
      // 清空输入缓冲区
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      syntron_servo.setLowLimit(lowlim);
      cout << "Set LOW LIMIT Position: " << lowlim << endl;

      // 使用 std::cout 提示用户输入速度限制
      std::cout << "Please input VELOCITY LIMIT.\n";
      // 循环验证用户输入，直到输入有效数字
      while (!(std::cin >> vellim))
      {
        std::cin.clear();                                                    // 清除错误标志
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // 忽略无效输入
        std::cout << "Invalid input. Please enter a valid number: ";
      }
      // 清空输入缓冲区
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      syntron_servo.setVelocity(vellim);
      cout << "Set VELOCITY LIMIT: " << vellim << endl;
    }
    if (ascii == 'i' || ascii == 'I')
    {
      cout << "Motor  Position:  " << syntron_servo.position << "  Velocity:  " << syntron_servo.velocity
           << "  In place?  " << syntron_servo.inplace << endl;
      cout << "Motor  Limit:  " << syntron_servo.highlimit << " " << syntron_servo.lowlimit << endl;
    }
    if (ascii == 'w' || ascii == 'W')
    {
      syntron_servo.setPosition(syntron_servo.position + 1);
      cout << "Motor Move Forward." << endl;
    }
    if (ascii == 's' || ascii == 'S')
    {
      syntron_servo.setPosition(syntron_servo.position - 1);
      cout << "Motor Move Backward." << endl;
    }
    if (ascii == 'o' || ascii == 'O')
    {
      r = syntron_servo.setZeroPoint();
      cout << "Set Motor Zero Point. " << r << endl;
    }
    if (ascii == 'b' || ascii == 'B')
    {
      syntron_servo.setStop();
      cout << "Motor Stop." << endl;
    }
    if (ascii == 'm' || ascii == 'M')
    {
      syntron_servo.start();
      cout << "Motor Enable." << endl;
    }
    if (ascii == 'n' || ascii == 'N')
    {
      syntron_servo.stop();
      cout << "Motor Disable." << endl;
    }
    if (ascii == 'r' || ascii == 'R')
    {
      cout << "Motor start reseting." << endl;
      syntron_servo.resetDriver();
      cout << "Motor reseted." << endl;
    }
    if (ascii == 'h' || ascii == 'H')
    {
      showmenu();
    }
    if (ascii == 'q' || ascii == 'Q')
    {
      syntron_servo.stop();
      cout << "Program Exit." << endl;
      break;
    }
  }
  return 0;
}
