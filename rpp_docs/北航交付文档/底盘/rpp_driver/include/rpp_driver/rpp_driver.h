#ifndef __H_RPP_DRIVER_H__
#define __H_RPP_DRIVER_H__

#include <array>
#include <thread>
#include <queue>
#include <condition_variable>

#include "ring_buffer.h"
#include "command.h"
#include "data_type.h"

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>

using namespace std;

#define DATA_LEN_MAX 1024 * 4 // 最大数据长度

#define MotionModel_2WD "2wd"
#define MotionModel_4WD "4wd"
#define MotionModel_Ackermann "ackermann"
#define MotionModel_Mecanum "mecanum"
#define MotionModel_4W4S "4w4s"

namespace rpp
{

class RPPDriver
{
  public:
    RPPDriver(string port, int32_t baud_rate, int32_t control_rate, string motion_model, double wheel_radius,
              double wheel_bias, double wheel_base, bool use_diff_twist);

    ~RPPDriver();

    void start();

    void stop();

    bool stoped();

    void recv_spin();

    void send_spin();

    void dataStream();

    void paraseData(char *data, int size);

    void handleMotorThetaStatus(const char *data, const int len);

    void handleMotorOmegaStatus(const char *data, const int len);

    void handleBatteryStatusData(const char *data, const int len);

    void handleErrStatusData(const char *data, const int len);

    void handleDevStatusData(const char *data, const int len);

    void handleLightStatusData(const char *data, const int len);

    void handleThrottleSteeringStatusData(const char *data, const int len);

    void updateOdometry(void);

    DeviceState getDeviceState();

    BatteryState getBatteryState();

    ErrorState getErrorState();

    LightState getLightState();

    MotorState getMotorState();

    MotorState getTurnMotorState();

    Odometry getOdometry();

    Twist getTwist();

    bool sendCommand(const vector<uint8_t> command);

    void sendControlCommand(const uint8_t control_mode, const uint8_t power_mode, const float motor_mode);

    void sendOmniVelocityCommand(const float linear_x, const float linear_y, const float angular_z);

    void sendVelocityCommand(const float linear_x, const float angular_z);

    void sendThrottleSteeringCommand(const uint8_t gear, const float throttle, const float brake,
                                     const float &constant_speed, const float &steering_angle_velocity,
                                     const float &steering_angular);

    void sendLightCommand(const uint8_t front_light, const uint8_t rear_light);

    void sendErrorMaskCommand(const uint8_t motor_mask, const uint8_t bump_mask);

  private:
    // serial
    boost::asio::io_context io_context_;
    boost::asio::serial_port *serial_port_ = nullptr;
    string port_;
    int baud_rate_;
    int control_rate_;

    // param
    string motion_model_; // 2wd 4wd ackermann mecanum 4w4s
    double wheel_radius_;
    double wheel_bias_;
    double wheel_base_;
    bool use_diff_twist_;

    // serial ring
    RingBuffer ring_buffer_;

    // buffer
    uint8_t handler_buf[DATA_LEN_MAX];
    //
    queue<vector<uint8_t>> send_queue_;
    std::mutex send_queue_mutex_;

    // thread
    bool loop_running_; // not atomic, maybe not safe
    thread recv_thread_;
    thread send_thread_;
    std::mutex send_mutex;
    std::condition_variable send_cv_;

    // device data
    DeviceState cur_dev_state_;
    std::mutex dev_state_mutex_;

    BatteryState cur_battery_state_;
    std::mutex battery_mutex_;

    LightState cur_light_state_;
    std::mutex light_mutex_;

    ErrorState cur_err_state_;
    std::mutex err_state_mutex_;

    // motor data
    MotorState cur_motor_state_;
    MotorState last_motor_state_;
    MotorState cur_turn_motor_state_;
    MotorState last_turn_motor_state_;
    std::mutex motor_mutex_;
    std::chrono::time_point<std::chrono::system_clock> last_odom_time_;
    std::chrono::time_point<std::chrono::system_clock> cur_odom_time_;
    Odometry odometry_;
    Twist twist_;
};

} // namespace rpp

#endif // __H_RPP_DRIVER_H__