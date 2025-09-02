#include <stdio.h>
#include <unistd.h>
#include <termio.h>

#include "rpp_driver/rpp_driver.h"

int scanKeyboard()
{
    int in;
    struct termios new_settings;
    struct termios stored_settings;
    tcgetattr(0, &stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0, &stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);

    in = getchar();

    tcsetattr(0, TCSANOW, &stored_settings);
    return in;
}

using namespace rpp;

int main()
{
    printf("Hello, World!\n");

    RPPDriver rpp_driver("/dev/ttyS0", 115200, 100, "4w4s", 0.156, 0.400, 0.450, true);
    rpp_driver.start();
    rpp_driver.sendControlCommand(0x01, 0, 0x0f);
    while (1)
    {
        usleep(100000);
        // printf("\033[2J\033[1;1H");
        auto dev_state = rpp_driver.getDeviceState();
        auto battery_state = rpp_driver.getBatteryState();
        auto motor_state = rpp_driver.getMotorState();
        auto turn_motor_state = rpp_driver.getTurnMotorState();
        printf("device_mode: %02x\n", dev_state.control_mode);
        // printf("motor_state: %0.4f\t %0.4f\t %0.4f\t %0.4f\t  \n", motor_state.motor_states[0].theta,
        //        motor_state.motor_states[1].theta, motor_state.motor_states[2].theta,
        //        motor_state.motor_states[3].theta);

        // rpp_driver.sendControlCommand(0x01, 0, 0x0f);
        int ascii = scanKeyboard();
        // printf("%d\n", ascii);
        if (ascii == 'q' || ascii == 'Q')
        {
            rpp_driver.sendOmniVelocityCommand(0.0, 0.0, 0.0);
            break;
        }
        if (ascii == 'f' || ascii == 'F')
        {
            rpp_driver.sendControlCommand(0x01, 0x0f, 0x0f);
        }
        if (ascii == 'r' || ascii == 'R')
        {
            rpp_driver.sendControlCommand(0x00, 0x05, 0x06);
        }
        if (ascii == 'w' || ascii == 'W')
        {
            rpp_driver.sendOmniVelocityCommand(0.1, 0.0, 0.0);
        }
        if (ascii == 'x' || ascii == 'X')
        {
            rpp_driver.sendOmniVelocityCommand(-0.1, 0.0, 0.0);
        }
        if (ascii == 'a' || ascii == 'A')
        {
            rpp_driver.sendOmniVelocityCommand(0.0, 0.0, 0.1);
        }
        if (ascii == 'd' || ascii == 'D')
        {
            rpp_driver.sendOmniVelocityCommand(0.0, 0.0, -0.1);
        }
        if (ascii == 's' || ascii == 'S')
        {
            rpp_driver.sendOmniVelocityCommand(0.0, 0.0, 0.0);
        }
    }
    rpp_driver.sendControlCommand(0x00, 0x00, 0x0f);
    rpp_driver.stop();
    return 0;
}