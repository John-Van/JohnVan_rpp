/**************************************************************************
 * Copyright (C), 2020-2025, Robot++ Co., Ltd.
 * Author: Damon
 * version: 0.1
 * Date: 2020-12-03
 *
 * Description: Command
 **************************************************************************/

#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <iostream>
#include <vector>

using namespace std;

#define MOTOR_COUNTS 4

// 电机 ID
#define LF_MOTOR_ID 0
#define RF_MOTOR_ID 1
#define LB_MOTOR_ID 2
#define RB_MOTOR_ID 3

#define CONTROL_MODE_COMMAND 0x04
#define THROTTLE_STEERING_COMMAND 0x0C
#define OMNI_VELOCITY_COMMAND 0x0E
#define VELOCITY_COMMAND 0x0F
#define LIGHT_COMMAND 0x15
#define ERROR_MASK_COMMAND 0x26
#define SYNC_COMMAND 0x70 // 同步

// Status command
/* 电机相关状态 */
#define MOTOR_THETA_STATUS 0x90 // 电机位置(弧度)
#define MOTOR_OMEGA_STATUS 0x91 // 电机速度(角速度)
#define ODOMETRY_DATA 0x9b      // odometry data
#define BATTERY_STATUS 0xed
#define ERROR_STATUS 0xf0
#define DEVICE_STATUS 0xf4
#define LIGHT_STATUS 0xf5
#define THROTTLE_STEERING_STATUS 0xfc

unsigned short getChecksum(vector<uint8_t> &cmdStream);

namespace command
{

vector<uint8_t> controlModeCmd(const uint8_t model, uint8_t power, const uint8_t motor);
vector<uint8_t> omniVelocityControlCmd(const float &linear_x, const float &linear_y, const float &angular);
vector<uint8_t> velocityControlCmd(const float &linear_x, const float &angular);
vector<uint8_t> throttleSteeringCmd(const uint8_t gear, const float throttle, const float brake,
                                    const float &constant_speed, const float &steering_angle_velocity,
                                    const float &steering_angular);
vector<uint8_t> lightStatusCmd(const uint8_t front, const uint8_t rear);
vector<uint8_t> errorMaskCmd(const uint8_t motor, const uint8_t bump);
vector<uint8_t> heartsCmd();

} // namespace command
#endif
