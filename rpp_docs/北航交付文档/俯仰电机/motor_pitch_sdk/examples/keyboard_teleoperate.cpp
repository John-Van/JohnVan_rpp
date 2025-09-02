#include <stdio.h>
#include <unistd.h>
#include <termio.h>
#include <iostream>
#include <iomanip>
#include <limits>
#include "motor_pitch/motor_pitch.h"

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

void show_menu()
{
  cout << "Motor Test" << endl;
  cout << "P: Set motor position." << endl;
  cout << "W: Motor move forward." << endl;
  cout << "S: Motor move backward." << endl;
  cout << "B: Motor stop." << endl;
  cout << "O: Set motor zero point." << endl;
  cout << "L: Set motor limits." << endl;
  cout << "I: Show motor info." << endl;
  cout << "M: Enable motor." << endl;
  cout << "N: Disable motor." << endl;
  cout << "C: Set current before move." << endl;
  cout << "H: Menu." << endl;
  cout << "Q: Program exit." << endl;
}

int main()
{
  Ti5CRA_Servo ti5cra_servo("can0", 0x01);
  ti5cra_servo.start();
  cout << boolalpha << fixed << setprecision(2);
  show_menu();
  while (1)
  {
    int ascii = scanKeyboard();
    if (ascii == -1)
    {
      continue;
    }
    if (ascii == 'p' || ascii == 'P')
    {
      // 使用 std::cout 提示用户输入目标位置（单位：度）
      std::cout << "Please input target position(degree).\n";
      float deg;
      // 循环验证用户输入，直到输入有效数字
      while (!(std::cin >> deg))
      {
        std::cin.clear();                                                    // 清除错误标志
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // 忽略无效输入
        std::cout << "Invalid input. Please enter a valid number: ";
      }
      // 清空输入缓冲区
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      ti5cra_servo.setPosition(deg);
      cout << "Set Position: " << deg << endl;
      if (deg > ti5cra_servo.highlimit || deg < ti5cra_servo.lowlimit)
      {
        cout << "Position out of limit, motor move to the nearest limit." << endl;
      }
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
      ti5cra_servo.setHighLimit(highlim);
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
      ti5cra_servo.setLowLimit(lowlim);
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
      ti5cra_servo.setVelocity(vellim);
      cout << "Set VELOCITY LIMIT: " << vellim << endl;
    }
    if (ascii == 'c' || ascii == 'C')
    {
      // 使用 std::cout 提示用户输入目标位置（单位：度）
      std::cout << "Please input current(A) before move.\n";
      float cur;
      // 循环验证用户输入，直到输入有效数字
      while (!(std::cin >> cur))
      {
        std::cin.clear();                                                    // 清除错误标志
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // 忽略无效输入
        std::cout << "Invalid input. Please enter a valid number: ";
      }
      // 清空输入缓冲区
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      ti5cra_servo.setCurrentBeforeMove(cur);
      cout << "Set Current Before Move: " << cur << endl;
    }
    if (ascii == 'i' || ascii == 'I')
    {
      cout << "Motor  Position:  " << ti5cra_servo.position << "  Velocity:  " << ti5cra_servo.velocity
           << "  In place?  " << ti5cra_servo.inplace << endl;
      cout << "Motor  Limit:  " << ti5cra_servo.highlimit << " " << ti5cra_servo.lowlimit << endl;
    }
    if (ascii == 'w' || ascii == 'W')
    {
      ti5cra_servo.moveForward(5);
      cout << "Motor Move Forward." << endl;
    }
    if (ascii == 's' || ascii == 'S')
    {
      ti5cra_servo.moveBackward(5);
      cout << "Motor Move Backward." << endl;
    }
    if (ascii == 'o' || ascii == 'O')
    {
      ti5cra_servo.setZeroPoint();
      cout << "Set Motor Zero Point." << endl;
    }
    if (ascii == 'b' || ascii == 'B')
    {
      ti5cra_servo.setStop();
      cout << "Motor Stop." << endl;
    }
    if (ascii == 'm' || ascii == 'M')
    {
      ti5cra_servo.start();
      cout << "Motor Enable." << endl;
    }
    if (ascii == 'n' || ascii == 'N')
    {
      ti5cra_servo.stop();
      cout << "Motor Disable." << endl;
    }
    if (ascii == 'h' || ascii == 'H')
    {
      show_menu();
    }
    if (ascii == 'q' || ascii == 'Q')
    {
      ti5cra_servo.setStop();
      cout << "Program Exit." << endl;
      break;
    }
  }
  return 0;
}