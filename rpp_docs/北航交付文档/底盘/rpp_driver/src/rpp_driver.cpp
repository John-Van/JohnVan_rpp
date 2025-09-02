#include <cmath>
#include <assert.h>
#include "rpp_driver/rpp_driver.h"

using namespace std;

namespace rpp
{

RPPDriver::RPPDriver(string port, int32_t baud_rate, int32_t control_rate, string motion_model,
                     double wheel_radius, double wheel_bias, double wheel_base, bool use_diff_twist)
    : port_(port), baud_rate_(baud_rate), control_rate_(control_rate), motion_model_(motion_model),
      wheel_radius_(wheel_radius), wheel_bias_(wheel_bias), wheel_base_(wheel_base), use_diff_twist_(use_diff_twist),
      loop_running_(false), serial_port_(nullptr)
{
    ring_buffer_.initSerialRingBuffer(DATA_LEN_MAX);
}

RPPDriver::~RPPDriver()
{
    loop_running_ = false;
    recv_thread_.join();
    send_thread_.join();
    if (serial_port_)
    {
        try
        {
            serial_port_->close();
        }
        catch (...)
        {
        }
        delete serial_port_;
        serial_port_ = nullptr;
    }
}

void RPPDriver::start()
{
    try
    {
        serial_port_ = new boost::asio::serial_port(io_context_, port_);
        serial_port_->set_option(boost::asio::serial_port::baud_rate(baud_rate_));
        serial_port_->set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none));
        serial_port_->set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none));
        serial_port_->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one));
        serial_port_->set_option(boost::asio::serial_port::character_size(8));

        loop_running_ = true;
        recv_thread_ = thread(&RPPDriver::recv_spin, this);
        send_thread_ = thread(&RPPDriver::send_spin, this);
    }
    catch (const std::exception &e)
    {
        fprintf(stderr, "Serial init failed: %s\n", e.what());
    }
}

void RPPDriver::stop()
{
    loop_running_ = false;
    send_cv_.notify_one();
}

bool RPPDriver::stoped()
{
    return loop_running_ == false && serial_port_ == nullptr;
}

void RPPDriver::send_spin()
{
    while (true)
    {
        unique_lock<mutex> lock(send_mutex);
        send_cv_.wait_for(lock, chrono::milliseconds(1000 / control_rate_),
                          [this] { return !send_queue_.empty() || !loop_running_; });
        {
            lock_guard<mutex> lock(send_queue_mutex_);
            if (send_queue_.empty())
            {
                auto heart_msg = command::heartsCmd();
                send_queue_.push(heart_msg);
            }
            while (!send_queue_.empty())
            {
                vector<uint8_t> msg;
                msg = send_queue_.front();
                send_queue_.pop();
                try
                {
                    size_t bytes_written =
                        boost::asio::write(*serial_port_, boost::asio::buffer(msg.data(), msg.size()));
                    if (bytes_written != msg.size())
                    {
                        fprintf(stderr, "Partial write: %zu/%zu\n", bytes_written, msg.size());
                    }
                }
                catch (const std::exception &e)
                {
                    fprintf(stderr, "Write failed: %s\n", e.what());
                }
            }
        }
        if (!loop_running_)
            break;
    }
}
void RPPDriver::recv_spin()
{
    while (loop_running_)
    {
        try
        {
            uint8_t buffer[1];
            size_t len =
                boost::asio::read(*serial_port_, boost::asio::buffer(buffer), boost::asio::transfer_at_least(1));
            if (len > 0)
            {
                for (size_t i = 0; i < len; ++i)
                {
                    // printf("%02x ", buffer[i]);
                    ring_buffer_.writeSerialRingBuffer(buffer[i]);
                }
                dataStream();
            }
        }
        catch (const boost::system::system_error &e)
        {
            // 处理超时或错误
            if (e.code() != boost::asio::error::operation_aborted)
            {
                fprintf(stderr, "Serial read error: %s\n", e.what());
                // 等待后重试
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
        catch (...)
        {
            // 其他异常
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

void RPPDriver::dataStream()
{
    static int recv_flag = 0;
    static int offset = 6;
    uint8_t data = 0;

    if (ring_buffer_.readSerialRingBuffer(&data))
    {
        switch (recv_flag)
        {
        case 0: {
            if (data == 0xFF)
            {
                handler_buf[0] = data;
                recv_flag = 1;
            }
            else
            {
                recv_flag = 0;
            }
            break;
        }
        case 1: {
            if (data == 0xAA)
            {
                handler_buf[1] = data;
                recv_flag = 2;
            }
            else
            {
                recv_flag = 0;
            }
            break;
        }
        case 2: {
            handler_buf[2] = data; // 从机地址
            recv_flag = 3;
            break;
        }
        case 3: {
            handler_buf[3] = data; // 功能码
            recv_flag = 4;
            break;
        }
        case 4: {
            handler_buf[4] = data; // 数据段字节数L
            recv_flag = 5;
            break;
        }
        case 5: {
            handler_buf[5] = data; // 数据段字节数H
            recv_flag = 6;
            break;
        }
        case 6: {
            handler_buf[offset] = data;
            uint16_t data_len = (uint16_t)(handler_buf[5] << 8 | handler_buf[4]);
            if (data_len < 1 || data_len > DATA_LEN_MAX || offset > sizeof(handler_buf))
            {
                fprintf(stderr, "error data_len: %d, offset: %d\n", data_len, offset);
                recv_flag = 0;
                offset = 6;
                break;
            }
            else if (offset >= (6 + data_len - 1))
            {
                recv_flag = 7;
            }
            offset++;
            break;
        }
        case 7: {
            handler_buf[offset] = data; // 校验值L
            offset++;
            recv_flag = 8;
            break;
        }
        case 8: {
            handler_buf[offset] = data; // 校验值H
            uint16_t recv_checksum = (uint16_t)(handler_buf[offset] << 8 | handler_buf[offset - 1]);
            vector<uint8_t> tmp(handler_buf, handler_buf + (offset - 1));
            uint16_t cal_checksum = getChecksum(tmp);
            if (cal_checksum == recv_checksum)
            {
                paraseData((char *)handler_buf, offset + 1); // 数据处理
            }
            recv_flag = 0;
            offset = 6;
            break;
        }
        default: {
            assert(false);
        }
        }
    }
}

void RPPDriver::paraseData(char *data, int size)
{
    if (size < 6)
    {
        return;
    }

    if (data[2] != 0x7f)
    {
        fprintf(stderr, "paraseData serial id. 0x%x\n", data[2]);
        return;
    }

    uint16_t data_len = (uint16_t)((uint8_t)data[5] << 8 | (uint8_t)data[4]);
    if (data_len + 8 > size)
    {
        fprintf(stderr, "Incomplete data length. Drop\n");
        return;
    }

    switch (static_cast<uint8_t>(data[3]))
    {
    case MOTOR_THETA_STATUS:
        handleMotorThetaStatus(data, size);
        break;
    case MOTOR_OMEGA_STATUS:
        handleMotorOmegaStatus(data, size);
        break;
    case BATTERY_STATUS:
        handleBatteryStatusData(data, size);
        break;
    case ERROR_STATUS:
        handleErrStatusData(data, size);
        break;
    case DEVICE_STATUS:
        handleDevStatusData(data, size);
        break;
    case LIGHT_STATUS:
        handleLightStatusData(data, size);
        break;
    case THROTTLE_STEERING_STATUS:
        handleThrottleSteeringStatusData(data, size);
        break;
    default:
        break;
    }
}

void RPPDriver::handleMotorThetaStatus(const char *data, const int len)
{
    lock_guard<mutex> lock(motor_mutex_);
    for (int i = 0; i < MOTOR_COUNTS; i++)
    {
        memcpy(&cur_motor_state_.motor_states[i].theta, (const void *)&data[4 * i + 6], sizeof(float));
    }
    if (motion_model_ == "4w4s")
    {
        for (int i = 0; i < MOTOR_COUNTS; i++)
        {
            memcpy(&cur_turn_motor_state_.motor_states[i].theta, (const void *)&data[4 * i + 6 + 16], sizeof(float));
        }
    }
    updateOdometry();
}

void RPPDriver::handleMotorOmegaStatus(const char *data, const int len)
{
    lock_guard<mutex> lock(motor_mutex_);
    for (int i = 0; i < MOTOR_COUNTS; i++)
    {
        memcpy(&cur_motor_state_.motor_states[i].omega, (const void *)&data[4 * i + 6], sizeof(float));
    }
    if (motion_model_ == "4w4s")
    {
        for (int i = 0; i < MOTOR_COUNTS; i++)
        {
            memcpy(&cur_turn_motor_state_.motor_states[i].omega, (const void *)&data[4 * i + 6 + 16], sizeof(float));
        }
    }
}

void RPPDriver::handleBatteryStatusData(const char *data, const int len)
{
    uint8_t percentage = 0;
    float temeprature = 0;
    float voltage = 0;
    float current = 0; // 负数表示放电
    memcpy(&percentage, (const void *)&data[6], sizeof(percentage));
    memcpy(&temeprature, (const void *)&data[7], sizeof(temeprature));
    memcpy(&voltage, (const void *)&data[11], sizeof(voltage));
    memcpy(&current, (const void *)&data[15], sizeof(current));

    lock_guard<mutex> lock(battery_mutex_);
    cur_battery_state_.percentage = percentage;
    cur_battery_state_.temperature = temeprature;
    cur_battery_state_.voltage = voltage;
    cur_battery_state_.current = current;
    cur_battery_state_.ts =
        chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

void RPPDriver::handleErrStatusData(const char *data, const int len)
{
    uint8_t motor_error;
    uint8_t turn_motor_error;
    uint8_t bump_error;
    uint8_t communication_error;
    memcpy(&motor_error, (const void *)&data[6], sizeof(motor_error));
    memcpy(&turn_motor_error, (const void *)&data[7], sizeof(turn_motor_error));
    memcpy(&bump_error, (const void *)&data[8], sizeof(bump_error));
    memcpy(&communication_error, (const void *)&data[9], sizeof(communication_error));
    lock_guard<mutex> lock(err_state_mutex_);
    cur_err_state_.motor_error = motor_error;
    cur_err_state_.turn_motor_error = turn_motor_error;
    cur_err_state_.bump_error = bump_error;
    cur_err_state_.communication_error = communication_error;
    cur_err_state_.ts =
        chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

void RPPDriver::handleDevStatusData(const char *data, const int len)
{
    uint8_t control_mode;
    uint8_t power_mode;
    uint8_t motor_mode;
    uint8_t e_stop;
    memcpy(&control_mode, (const void *)&data[6], sizeof(control_mode));
    memcpy(&power_mode, (const void *)&data[7], sizeof(power_mode));
    memcpy(&motor_mode, (const void *)&data[8], sizeof(motor_mode));
    memcpy(&e_stop, (const void *)&data[8], sizeof(e_stop));
    lock_guard<mutex> lock(dev_state_mutex_);
    cur_dev_state_.control_mode = control_mode;
    cur_dev_state_.power_mode = power_mode;
    cur_dev_state_.motor_mode = motor_mode;
    cur_dev_state_.e_stop = e_stop;
    cur_dev_state_.ts =
        chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

void RPPDriver::handleLightStatusData(const char *data, const int len)
{
    uint8_t front_light, rear_light;
    memcpy(&front_light, (const void *)&data[6], sizeof(front_light));
    memcpy(&rear_light, (const void *)&data[7], sizeof(rear_light));

    lock_guard<mutex> lock(light_mutex_);
    cur_light_state_.front_light = front_light;
    cur_light_state_.rear_light = rear_light;
    cur_light_state_.ts =
        chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

void RPPDriver::handleThrottleSteeringStatusData(const char *data, const int len)
{
    uint8_t gear_position = 0xFF;
    float throttle = 0;
    float brake_strength = 0;
    float speed = 0;
    float steering_angular = 0;
    float steering_angle_velocity = 0;
    memcpy(&gear_position, (const void *)&data[6], sizeof(gear_position));
    memcpy(&throttle, (const void *)&data[7], sizeof(throttle));
    memcpy(&brake_strength, (const void *)&data[11], sizeof(brake_strength));
    memcpy(&speed, (const void *)&data[15], sizeof(speed));
    memcpy(&steering_angular, (const void *)&data[19], sizeof(steering_angular));
    memcpy(&steering_angle_velocity, (const void *)&data[23], sizeof(int));
}

void RPPDriver::updateOdometry(void)
{
    // cur_motor_state_
    static bool init = false;
    static chrono::time_point<chrono::system_clock> last_time;

    if (!init)
    {
        /// TODO: wait for motor state from mobile base.
        last_motor_state_ = cur_motor_state_;
        if (motion_model_ == "4w4s")
            last_turn_motor_state_ = cur_turn_motor_state_;
        init = true;

        last_time = chrono::system_clock::now();
    }
    auto now = chrono::system_clock::now();
    auto dt = chrono::duration_cast<chrono::nanoseconds>(now - last_time).count() / 1000000000.0;

    if (dt < 0.00001)
    {
        return;
    }
    last_time = chrono::system_clock::now();

    float lf_diff_theta =
        cur_motor_state_.motor_states[LF_MOTOR_ID].theta - last_motor_state_.motor_states[LF_MOTOR_ID].theta;
    float lb_diff_theta =
        cur_motor_state_.motor_states[LB_MOTOR_ID].theta - last_motor_state_.motor_states[LB_MOTOR_ID].theta;
    float rf_diff_theta =
        cur_motor_state_.motor_states[RF_MOTOR_ID].theta - last_motor_state_.motor_states[RF_MOTOR_ID].theta;
    float rb_diff_theta =
        cur_motor_state_.motor_states[RB_MOTOR_ID].theta - last_motor_state_.motor_states[RB_MOTOR_ID].theta;

    double dist_lf = 0.0;
    double dist_lb = 0.0;
    double dist_rf = 0.0;
    double dist_rb = 0.0;
    double dx = 0.0;
    double dy = 0.0;
    double domega = 0.0;
    if (motion_model_ == "2wd")
    {
        dist_lf = wheel_radius_ * lf_diff_theta;
        dist_rf = wheel_radius_ * rf_diff_theta;
        dx = (dist_lf + dist_rf) * 0.5;
        domega = (dist_rf - dist_lf) / wheel_bias_;
    }
    else if (motion_model_ == "4wd")
    {
        dx = ((dist_lf + dist_lb) + (dist_rf + dist_rb)) * 0.25;
        domega = ((dist_rf + dist_rb) - (dist_lf + dist_lb)) * 0.5 / wheel_bias_;
    }
    else if (motion_model_ == "ackermann")
    {
        dist_lb = wheel_radius_ * lb_diff_theta;
        dist_rb = wheel_radius_ * rb_diff_theta;
        dx = (dist_lb + dist_rb) * 0.5;
        domega = (dist_rb - dist_lb) / wheel_bias_;
    }
    else if (motion_model_ == "mecanum")
    {
        dist_lf = wheel_radius_ * lf_diff_theta;
        dist_lb = wheel_radius_ * lb_diff_theta;
        dist_rf = wheel_radius_ * rf_diff_theta;
        dist_rb = wheel_radius_ * rb_diff_theta;
        dx = (dist_lf + dist_lb + dist_rf + dist_rb) * 0.25;
        dy = (-dist_lf + dist_lb + dist_rf - dist_rb) * 0.25;
        domega = (-dist_lf - dist_lb + dist_rf + dist_rb) * 0.25 / (wheel_base_ + wheel_bias_);
    }
    else if (motion_model_ == "4w4s")
    {
        float lf_turn_angle = cur_turn_motor_state_.motor_states[LF_MOTOR_ID].theta;
        float lb_turn_angle = cur_turn_motor_state_.motor_states[LB_MOTOR_ID].theta;
        float rf_turn_angle = cur_turn_motor_state_.motor_states[RF_MOTOR_ID].theta;
        float rb_turn_angle = cur_turn_motor_state_.motor_states[RB_MOTOR_ID].theta;
        float c_lf = cos(lf_turn_angle);
        float s_lf = sin(lf_turn_angle);
        float c_lb = cos(lb_turn_angle);
        float s_lb = sin(lb_turn_angle);
        float c_rf = cos(rf_turn_angle);
        float s_rf = sin(rf_turn_angle);
        float c_rb = cos(rb_turn_angle);
        float s_rb = sin(rb_turn_angle);
        dist_lf = wheel_radius_ * lf_diff_theta;
        dist_lb = wheel_radius_ * lb_diff_theta;
        dist_rf = wheel_radius_ * rf_diff_theta;
        dist_rb = wheel_radius_ * rb_diff_theta;
        float dist_lf_mult_c = dist_lf * c_lf;
        float dist_lf_mult_s = dist_lf * s_lf;
        float dist_lb_mult_c = dist_lb * c_lb;
        float dist_lb_mult_s = dist_lb * s_lb;
        float dist_rf_mult_c = dist_rf * c_rf;
        float dist_rf_mult_s = dist_rf * s_rf;
        float dist_rb_mult_c = dist_rb * c_rb;
        float dist_rb_mult_s = dist_rb * s_rb;
        float d_squared = wheel_base_ * wheel_base_ + wheel_bias_ * wheel_bias_;
        dx = (dist_lf_mult_c + dist_lb_mult_c + dist_rf_mult_c + dist_rb_mult_c) * 0.25;
        dy = (dist_lf_mult_s + dist_lb_mult_s + dist_rf_mult_s + dist_rb_mult_s) * 0.25;
        domega = (wheel_base_ * (dist_lf_mult_s - dist_lb_mult_s + dist_rf_mult_s - dist_rb_mult_s) * 0.5 +
                  wheel_bias_ * (-dist_lf_mult_c - dist_lb_mult_c + dist_rf_mult_c + dist_rb_mult_c) * 0.5) /
                 d_squared;
    }

    if (fabs(dx) > 1.6)
    {
        fprintf(stderr, "Invalid dx value[%lf].\n", dx);
        return;
    }

    odometry_.x += (dx * cos(odometry_.theta) - dy * sin(odometry_.theta));
    odometry_.y += (dx * sin(odometry_.theta) + dy * cos(odometry_.theta));
    odometry_.theta += domega;

    if (use_diff_twist_)
    {
        twist_.x = dx / dt;
        twist_.y = dy / dt;
        twist_.theta = domega / dt;
    }
    else
    {
        double vel_lf, vel_lb, vel_rf, vel_rb;
        vel_lf = cur_motor_state_.motor_states[LF_MOTOR_ID].omega * wheel_radius_;
        vel_lb = cur_motor_state_.motor_states[LB_MOTOR_ID].omega * wheel_radius_;
        vel_rf = cur_motor_state_.motor_states[RF_MOTOR_ID].omega * wheel_radius_;
        vel_rb = cur_motor_state_.motor_states[RB_MOTOR_ID].omega * wheel_radius_;

        if (motion_model_ == "2wd")
        {
            twist_.x = (vel_lf + vel_rf) * 0.5;
            twist_.theta = (vel_rf - vel_lf) / wheel_bias_;
        }
        else if (motion_model_ == "4wd")
        {
            twist_.x = (vel_lf + vel_lb + vel_rf + vel_rb) * 0.25;
            twist_.theta = ((vel_rf + vel_rb) - (vel_lf + vel_lb)) * 0.5 / wheel_bias_;
        }
        else if (motion_model_ == "ackermann")
        {
            twist_.x = (vel_lb + vel_rb) * 0.5;
            twist_.theta = (vel_rb - vel_lb) / wheel_bias_;
        }
        else if (motion_model_ == "mecanum")
        {
            twist_.x = (vel_lf + vel_lb + vel_rf + vel_rb) * 0.25;
            twist_.y = (-vel_lf + vel_lb + vel_rf - vel_rb) * 0.25;
            twist_.theta = (-vel_lf + vel_lb + vel_rf - vel_rb) * 0.25 / (wheel_base_ + wheel_bias_);
        }
        else if (motion_model_ == "4w4s")
        {
            float lf_turn_angle = cur_turn_motor_state_.motor_states[LF_MOTOR_ID].theta;
            float lb_turn_angle = cur_turn_motor_state_.motor_states[LB_MOTOR_ID].theta;
            float rf_turn_angle = cur_turn_motor_state_.motor_states[RF_MOTOR_ID].theta;
            float rb_turn_angle = cur_turn_motor_state_.motor_states[RB_MOTOR_ID].theta;
            float c_lf = cos(lf_turn_angle);
            float s_lf = sin(lf_turn_angle);
            float c_lb = cos(lb_turn_angle);
            float s_lb = sin(lb_turn_angle);
            float c_rf = cos(rf_turn_angle);
            float s_rf = sin(rf_turn_angle);
            float c_rb = cos(rb_turn_angle);
            float s_rb = sin(rb_turn_angle);
            float vel_lf_mult_c = vel_lf * c_lf;
            float vel_lf_mult_s = vel_lf * s_lf;
            float vel_lb_mult_c = vel_lb * c_lb;
            float vel_lb_mult_s = vel_lb * s_lb;
            float vel_rf_mult_c = vel_rf * c_rf;
            float vel_rf_mult_s = vel_rf * s_rf;
            float vel_rb_mult_c = vel_rb * c_rb;
            float vel_rb_mult_s = vel_rb * s_rb;
            float d_squared = wheel_base_ * wheel_base_ + wheel_bias_ * wheel_bias_;
            twist_.x = (vel_lf_mult_c + vel_lb_mult_c + vel_rf_mult_c + vel_rb_mult_c) * 0.25;
            twist_.y = (vel_lf_mult_s + vel_lb_mult_s + vel_rf_mult_s + vel_rb_mult_s) * 0.25;
            twist_.theta = (wheel_base_ * (vel_lf_mult_s - vel_lb_mult_s + vel_rf_mult_s - vel_rb_mult_s) * 0.5 +
                            wheel_bias_ * (-vel_lf_mult_c - vel_lb_mult_c + vel_rf_mult_c + vel_rb_mult_c) * 0.5) /
                           d_squared;
        }
    }
    odometry_.ts = twist_.ts = chrono::duration_cast<chrono::nanoseconds>(now.time_since_epoch()).count();
    last_motor_state_ = cur_motor_state_;
    if (motion_model_ == "4w4s")
        last_turn_motor_state_ = cur_turn_motor_state_;
}

DeviceState RPPDriver::getDeviceState()
{
    lock_guard<mutex> lock(dev_state_mutex_);
    return cur_dev_state_;
}

BatteryState RPPDriver::getBatteryState()
{
    lock_guard<mutex> lock(battery_mutex_);
    return cur_battery_state_;
}

ErrorState RPPDriver::getErrorState()
{
    lock_guard<mutex> lock(err_state_mutex_);
    return cur_err_state_;
}

LightState RPPDriver::getLightState()
{
    lock_guard<mutex> lock(light_mutex_);
    return cur_light_state_;
}

MotorState RPPDriver::getMotorState()
{
    lock_guard<mutex> lock(motor_mutex_);
    return cur_motor_state_;
}

MotorState RPPDriver::getTurnMotorState()
{
    lock_guard<mutex> lock(motor_mutex_);
    return cur_turn_motor_state_;
}

Odometry RPPDriver::getOdometry()
{
    lock_guard<mutex> lock(motor_mutex_);
    return odometry_;
}

Twist RPPDriver::getTwist()
{
    lock_guard<mutex> lock(motor_mutex_);
    return twist_;
}

bool RPPDriver::sendCommand(const vector<uint8_t> command)
{
    lock_guard<mutex> lock(send_queue_mutex_);
    send_queue_.push(command);
    send_cv_.notify_one();
    return true;
}

void RPPDriver::sendControlCommand(const uint8_t control_mode, const uint8_t power_mode, const float motor_mode)
{
    auto cmd = command::controlModeCmd(control_mode, power_mode, motor_mode);
    sendCommand(cmd);
}

void RPPDriver::sendOmniVelocityCommand(const float linear_x, const float linear_y, const float angular_z)
{
    auto cmd = command::omniVelocityControlCmd(linear_x, linear_y, angular_z);
    sendCommand(cmd);
}

void RPPDriver::sendVelocityCommand(const float linear_x, const float angular_z)
{
    auto cmd = command::velocityControlCmd(linear_x, angular_z);
    sendCommand(cmd);
}

void RPPDriver::sendThrottleSteeringCommand(const uint8_t gear, const float throttle, const float brake,
                                            const float &constant_speed, const float &steering_angle_velocity,
                                            const float &steering_angular)
{
    auto cmd =
        command::throttleSteeringCmd(gear, throttle, brake, constant_speed, steering_angle_velocity, steering_angular);
    sendCommand(cmd);
}

void RPPDriver::sendLightCommand(const uint8_t front, const uint8_t rear)
{
    auto cmd = command::lightStatusCmd(front, rear);
    sendCommand(cmd);
}

void RPPDriver::sendErrorMaskCommand(const uint8_t motor, const uint8_t bump)
{
    auto cmd = command::errorMaskCmd(motor, bump);
    sendCommand(cmd);
}

} // namespace rpp