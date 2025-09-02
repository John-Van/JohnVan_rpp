#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <csignal>
#include "rpp_driver/rpp_driver.h"

using namespace std;
using namespace rpp;

// 全局变量
static struct termios original_term;
static int original_flags;
bool program_running = true;

// 设置终端为非阻塞模式
void set_nonblocking_terminal()
{
    tcgetattr(STDIN_FILENO, &original_term);
    original_flags = fcntl(STDIN_FILENO, F_GETFL);

    struct termios new_term = original_term;
    new_term.c_lflag &= ~(ICANON | ECHO); // 禁用行缓冲和回显
    new_term.c_cc[VMIN] = 0;              // 非阻塞模式
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
    fcntl(STDIN_FILENO, F_SETFL, original_flags | O_NONBLOCK);
}

// 恢复终端原始设置
void restore_terminal()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_term);
    fcntl(STDIN_FILENO, F_SETFL, original_flags);
}

// Ctrl+C 信号处理
void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        printf("\n收到 Ctrl+C，安全停止中...\n");
        program_running = false;
    }
}

int main()
{
    signal(SIGINT, signal_handler);
    set_nonblocking_terminal();
    atexit(restore_terminal);

    RPPDriver rpp_driver("/dev/ttyUSB0", 230400, 100, "mecanum", 0.156, 0.400, 0.450, true);
    rpp_driver.start();
    rpp_driver.sendControlCommand(0x01, 0, 0x0f);

    int counter = 0;
    while (program_running)
    {
        usleep(10000); // 10ms延迟
        char key;

        // 处理按键输入
        if (read(STDIN_FILENO, &key, 1) > 0 && key > 0)
        {
            if (key == 'q' || key == 'Q')
            {
                program_running = false; // 设置退出标志
                break;
            }

            // 使用switch处理按键命令
            switch (key)
            {
            case 'f':
            case 'F':
                rpp_driver.sendControlCommand(0x01, 0x0f, 0x0f);
                break;
            case 'r':
            case 'R':
                rpp_driver.sendControlCommand(0x00, 0x05, 0x06);
                break;
            case 'w':
            case 'W':
                rpp_driver.sendOmniVelocityCommand(0.1, 0.0, 0.0);
                break;
            case 'x':
            case 'X':
                rpp_driver.sendOmniVelocityCommand(-0.1, 0.0, 0.0);
                break;
            case 'a':
            case 'A':
                rpp_driver.sendOmniVelocityCommand(0.0, 0.0, 0.1);
                break;
            case 'd':
            case 'D':
                rpp_driver.sendOmniVelocityCommand(0.0, 0.0, -0.1);
                break;
            case 'z':
            case 'Z':
                rpp_driver.sendOmniVelocityCommand(0.0, 0.1, 0.0);
                break;
            case 'c':
            case 'C':
                rpp_driver.sendOmniVelocityCommand(0.0, -0.1, 0.0);
                break;
            case 's':
            case 'S':
                rpp_driver.sendOmniVelocityCommand(0.0, 0.0, 0.0);
                break;
            }
        }

        // 每秒更新一次状态显示
        if (++counter >= 100)
        {
            counter = 0;
            auto dev_state = rpp_driver.getDeviceState();
            auto battery = rpp_driver.getBatteryState();
            auto motors = rpp_driver.getMotorState();
            auto turn_motors = rpp_driver.getTurnMotorState();

            printf("\033[2J\033[1;1H=========== 机器人状态 ===========\n");
            printf("控制模式: %02x\n电池: %d%% %.2fV %.2fA\n", dev_state.control_mode, battery.percentage,
                   battery.voltage, battery.current);
            printf("电机: %.2f %.2f %.2f %.2f\n转向: %.2f %.2f %.2f %.2f\n", motors.motor_states[0].theta,
                   motors.motor_states[1].theta, motors.motor_states[2].theta, motors.motor_states[3].theta,
                   turn_motors.motor_states[0].theta, turn_motors.motor_states[1].theta,
                   turn_motors.motor_states[2].theta, turn_motors.motor_states[3].theta);
            printf("=========== 控制指令 ============\n"
                   "W:前进 X:后退 A:左转 D:右转\n"
                   "Z:左侧 C:右侧 S:停止\n"
                   "F:强制模式 R:释放模式 Q:退出\n");
        }
    }

    // 安全退出流程
    printf("安全停止中...\n");

    // 确保发送停止运动命令
    rpp_driver.sendOmniVelocityCommand(0.0, 0.0, 0.0);

    // 最多尝试5次释放控制
    for (int i = 0; i < 50; i++)
    {
        rpp_driver.sendControlCommand(0x00, 0x00, 0x0f);
        usleep(100000);

        auto dev_state = rpp_driver.getDeviceState();
        printf("控制模式: %02x\n", dev_state.control_mode);

        if ((dev_state.control_mode & 0x01) == 0x00)
            break;
    }

    rpp_driver.stop();
    printf("程序已安全退出\n");
    return 0;
}