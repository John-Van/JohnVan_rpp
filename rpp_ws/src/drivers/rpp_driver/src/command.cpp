/**************************************************************************
 * Copyright (C), 2020-2025, Robot++ Co., Ltd.
 * Author: Damon
 * version: 0.1
 * Date: 2020-12-03
 *
 * Description: Command
 **************************************************************************/

#include <cstring>
#include "rpp_driver/command.h"

using namespace std;

unsigned short getChecksum(vector<uint8_t> &cmdStream)
{
    unsigned short checksum = 0xFFFF;
    unsigned short tmp = 0;
    for (unsigned int i = 0; i < cmdStream.size(); i++)
    {
        checksum ^= (cmdStream[i]);
        for (unsigned int j = 0; j < 8; j++)
        {
            if (checksum & 0x01)
            {
                checksum = (checksum >> 1) ^ 0xa001;
            }
            else
            {
                checksum = checksum >> 1;
            }
        }
    }

    return checksum;
}

template <typename T> void buildBytes(vector<unsigned char> &buffer, const T &V)
{
    unsigned int size_value(sizeof(T));
    for (unsigned int i = 0; i < size_value; i++)
    {
        buffer.push_back(static_cast<unsigned char>((V >> (i * 8)) & 0xff));
    }
}

template <> void buildBytes<float>(vector<unsigned char> &buffer, const float &V)
{
    uint32_t intVal;
    std::memcpy(&intVal, &V, sizeof(float));

    constexpr size_t size_value = sizeof(float);
    for (size_t i = 0; i < size_value; ++i)
    {
        buffer.push_back(static_cast<unsigned char>((intVal >> (i * 8)) & 0xff));
    }
}

template <> void buildBytes<vector<uint8_t>>(vector<unsigned char> &buffer, const vector<uint8_t> &V)
{
    buffer.insert(buffer.end(), V.begin(), V.end());
}

vector<uint8_t> buildCmd(uint8_t code, uint16_t len, vector<uint8_t> data)
{
    vector<uint8_t> cmd;
    cmd.push_back(static_cast<uint8_t>(0xff));
    cmd.push_back(static_cast<uint8_t>(0xaa));
    cmd.push_back(static_cast<uint8_t>(0x7f));
    cmd.push_back(static_cast<uint8_t>(code));
    buildBytes(cmd, static_cast<uint16_t>(len));
    buildBytes(cmd, data);
    buildBytes(cmd, getChecksum(cmd));
    return cmd;
}

namespace command
{

vector<uint8_t> controlModeCmd(uint8_t model, uint8_t power, uint8_t motor)
{
    vector<uint8_t> control_cmd;
    buildBytes(control_cmd, static_cast<uint8_t>(model));
    buildBytes(control_cmd, static_cast<uint8_t>(0x0f)); // power 电源管理位，暂时固定写为 0x0f，即:全部上电
    buildBytes(control_cmd, static_cast<uint8_t>(motor));
    buildBytes(control_cmd, static_cast<uint8_t>(0x00));
    buildBytes(control_cmd, static_cast<uint32_t>(0x00000000));
    return buildCmd(static_cast<uint8_t>(0x04), static_cast<uint8_t>(0x08), control_cmd);
}

vector<uint8_t> omniVelocityControlCmd(const float &linear_x, const float &linear_y, const float &angular)
{
    vector<uint8_t> vel_cmd;
    buildBytes(vel_cmd, static_cast<float>(linear_x));
    buildBytes(vel_cmd, static_cast<float>(linear_y));
    buildBytes(vel_cmd, static_cast<float>(angular));
    return buildCmd(static_cast<uint8_t>(0x0e), static_cast<uint8_t>(0x0c), vel_cmd);
}

vector<uint8_t> velocityControlCmd(const float &linear_x, const float &angular)
{
    vector<uint8_t> vel_cmd;
    buildBytes(vel_cmd, static_cast<float>(linear_x));
    buildBytes(vel_cmd, static_cast<float>(angular));
    return buildCmd(static_cast<uint8_t>(0x0f), static_cast<uint8_t>(0x08), vel_cmd);
}

vector<uint8_t> throttleSteeringCmd(const uint8_t gear, const float throttle, const float brake,
                                    const float &constant_speed, const float &steering_angle_velocity,
                                    const float &steering_angular)
{
    vector<uint8_t> throttle_steering_cmd;
    buildBytes(throttle_steering_cmd, static_cast<uint8_t>(gear));
    buildBytes(throttle_steering_cmd, static_cast<float>(throttle));
    buildBytes(throttle_steering_cmd, static_cast<float>(brake));
    buildBytes(throttle_steering_cmd, static_cast<float>(constant_speed));
    buildBytes(throttle_steering_cmd, static_cast<float>(steering_angle_velocity));
    buildBytes(throttle_steering_cmd, static_cast<float>(steering_angular));
    return buildCmd(static_cast<uint8_t>(0x0c), static_cast<uint8_t>(0x15), throttle_steering_cmd);
}

vector<uint8_t> lightStatusCmd(const uint8_t front, const uint8_t rear)
{
    vector<uint8_t> light_control_cmd;
    buildBytes(light_control_cmd, static_cast<uint8_t>(front));
    buildBytes(light_control_cmd, static_cast<uint8_t>(rear));
    buildBytes(light_control_cmd, static_cast<uint8_t>(0x00));
    buildBytes(light_control_cmd, static_cast<uint8_t>(0x00));
    buildBytes(light_control_cmd, static_cast<uint32_t>(0x00000000));
    return buildCmd(static_cast<uint8_t>(0x15), static_cast<uint8_t>(0x08), light_control_cmd);
}

vector<uint8_t> errorMaskCmd(const uint8_t motor, const uint8_t bump)
{
    vector<uint8_t> mask_cmd;
    buildBytes(mask_cmd, static_cast<uint8_t>(motor));
    buildBytes(mask_cmd, static_cast<uint8_t>(0xff));
    buildBytes(mask_cmd, static_cast<uint8_t>(bump));
    buildBytes(mask_cmd, static_cast<uint8_t>(0xff));
    buildBytes(mask_cmd, static_cast<uint32_t>(0xffffffff));
    return buildCmd(static_cast<uint8_t>(0x26), static_cast<uint8_t>(0x08), mask_cmd);
}

vector<uint8_t> heartsCmd()
{
    vector<uint8_t> hearts_cmd;
    buildBytes(hearts_cmd, static_cast<uint32_t>(0x0));
    buildBytes(hearts_cmd, static_cast<uint32_t>(0x0));
    return buildCmd(static_cast<uint8_t>(0x70), static_cast<uint8_t>(0x08), hearts_cmd);
}

} // namespace command