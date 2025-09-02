#include "rpp_ros_driver/rpp_ros_driver.h"

#include <functional>

namespace rpp
{

RPPRosDriver::RPPRosDriver() : Node("serial_driver"), rpp_driver_(nullptr)
{
    this->declare_parameter<std::string>("port", "/dev/ttyS0");
    this->declare_parameter<int>("baud_rate", 115200);
    this->declare_parameter<int>("control_rate", 20);
    this->declare_parameter<std::string>("odom_frame", std::string("odom"));
    this->declare_parameter<std::string>("base_frame", std::string("base_link"));
    this->declare_parameter<double>("wheel_radius", 0.08);
    this->declare_parameter<double>("wheel_bias", 0.355);
    this->declare_parameter<double>("wheel_base", 0.80);
    this->declare_parameter<bool>("publish_odom", true);
    this->declare_parameter<bool>("publish_odom_tf", true);
    this->declare_parameter<bool>("use_diff_twist", true);
    this->declare_parameter<std::string>("motion_model", std::string("2wd"));
    port_name_ = this->get_parameter("port").get_value<std::string>();
    baud_rate_ = this->get_parameter("baud_rate").get_value<int>();
    control_rate_ = this->get_parameter("control_rate").get_value<int>();
    odom_frame_ = this->get_parameter("odom_frame").get_value<std::string>();
    base_frame_ = this->get_parameter("base_frame").get_value<std::string>();
    wheel_radius_ = this->get_parameter("wheel_radius").get_value<double>();
    wheel_bias_ = this->get_parameter("wheel_bias").get_value<double>();
    wheel_base_ = this->get_parameter("wheel_base").get_value<double>();
    publish_odom_ = this->get_parameter("publish_odom").get_value<bool>();
    publish_odom_tf_ = this->get_parameter("publish_odom_tf").get_value<bool>();
    use_diff_twist_ = this->get_parameter("use_diff_twist").get_value<bool>();
    motion_model_ = this->get_parameter("motion_model").get_value<std::string>();

    odom_.header.frame_id = odom_frame_;
    odom_.child_frame_id = base_frame_;

    RCLCPP_INFO(this->get_logger(), "motion model: %s", motion_model_.c_str());
    rpp_driver_ = new RPPDriver(port_name_, baud_rate_, control_rate_, motion_model_, wheel_radius_, wheel_bias_,
                                wheel_base_, use_diff_twist_);
    rpp_driver_->start();
    while (!(rpp_driver_->getDeviceState().control_mode & 0x01))
    {
        rpp_driver_->sendControlCommand(0x01, 0x0f, 0x0f);
        RCLCPP_INFO(this->get_logger(), "enable serial control %02x", rpp_driver_->getDeviceState().control_mode);
        rclcpp::sleep_for(std::chrono::milliseconds(100));
    }
    RCLCPP_INFO(this->get_logger(), "enable serial control success");

    set_mode_sub_ = this->create_subscription<rpp_msgs::msg::SetDevice>(
        "set_device", 5, std::bind(&RPPRosDriver::setDeviceCallback, this, std::placeholders::_1));

    set_light_sub_ = this->create_subscription<rpp_msgs::msg::SetLight>(
        "set_light", 5, std::bind(&RPPRosDriver::setLightCallback, this, std::placeholders::_1));

    set_error_mask_sub_ = this->create_subscription<rpp_msgs::msg::SetErrorMask>(
        "set_error_mask", 5, std::bind(&RPPRosDriver::setErrorMaskCallback, this, std::placeholders::_1));

    twist_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
        "/cmd_vel", 5, std::bind(&RPPRosDriver::twistCallback, this, std::placeholders::_1));

    device_pub_ = this->create_publisher<rpp_msgs::msg::DeviceState>("device_state", 10);
    battery_pub_ = this->create_publisher<rpp_msgs::msg::BatteryState>("battrey_state", 10);
    light_pub_ = this->create_publisher<rpp_msgs::msg::LightState>("light_state", 10);
    error_pub_ = this->create_publisher<rpp_msgs::msg::ErrorState>("error_state", 10);
    motor_pub_ = this->create_publisher<rpp_msgs::msg::MotorState>("motor_state", 10);
    trun_motor_pub_ = this->create_publisher<rpp_msgs::msg::MotorState>("trun_motor_state", 10);
    twist_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/twist", 10);
    odom_pub_ = this->create_publisher<nav_msgs::msg::Odometry>("/odom", 10);

    last_twist_time_ = this->now();
    odom_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(this);
    publish_data_timer_ = this->create_wall_timer(10ms, std::bind(&RPPRosDriver::publishData, this)); // 100hz
}

RPPRosDriver::~RPPRosDriver()
{
    if (rpp_driver_ != nullptr)
    {
        while ((rpp_driver_->getDeviceState().control_mode & 0x01))
        {
            rpp_driver_->sendControlCommand(0x00, 0x0f, 0x0f);
            RCLCPP_INFO(this->get_logger(), "disable serial control %02x", rpp_driver_->getDeviceState().control_mode);
            rclcpp::sleep_for(std::chrono::milliseconds(100));
        }
        RCLCPP_INFO(this->get_logger(), "disable serial control success");
        rpp_driver_->stop();
        RCLCPP_INFO(this->get_logger(), "wait device stop");
        while (!rpp_driver_->stoped())
        {
            RCLCPP_INFO(this->get_logger(), "wait device stop");
            rclcpp::sleep_for(std::chrono::milliseconds(100));
        }
        RCLCPP_INFO(this->get_logger(), "device stop");
        delete rpp_driver_;
    }
}

void RPPRosDriver::setDeviceCallback(const rpp_msgs::msg::SetDevice::SharedPtr cmd)
{
    if (rpp_driver_ == nullptr)
        return;
    rpp_driver_->sendControlCommand(cmd->control_mode, 0x0f, 0x0f);
}

void RPPRosDriver::setLightCallback(const rpp_msgs::msg::SetLight::SharedPtr cmd)
{
    if (rpp_driver_ == nullptr)
        return;
    rpp_driver_->sendLightCommand(cmd->front_light, cmd->rear_light);
}

void RPPRosDriver::setErrorMaskCallback(const rpp_msgs::msg::SetErrorMask::SharedPtr cmd)
{
    if (rpp_driver_ == nullptr)
        return;
    rpp_driver_->sendErrorMaskCommand(cmd->motor_mask, cmd->bump_mask);
}

void RPPRosDriver::twistCallback(const geometry_msgs::msg::Twist::SharedPtr cmd)
{
    if (rpp_driver_ == nullptr)
        return;
    rpp_driver_->sendOmniVelocityCommand(cmd->linear.x, cmd->linear.y, cmd->angular.z);
    last_twist_time_ = this->now();
}

void RPPRosDriver::publishData()
{
    if (rpp_driver_ == nullptr)
        return;
    if ((this->now() - last_twist_time_).seconds() >= 0.5)
    {
        rpp_driver_->sendOmniVelocityCommand(0, 0, 0);
    }
    // 100hz
    static int tick_count = 0;
    if (tick_count++ % 10 == 0)
    {
        if (tick_count >= 100)
            tick_count = 0;
        if (device_pub_->get_subscription_count() != 0)
        {
            auto device_state = rpp_driver_->getDeviceState();
            device_state_.control_mode = device_state.control_mode;
            device_state_.power_mode = device_state.power_mode;
            device_state_.motor_mode = device_state.motor_mode;
            device_state_.e_stop = device_state.e_stop;
            device_state_.ts = device_state.ts;
            device_pub_->publish(device_state_);
        }

        if (battery_pub_->get_subscription_count() != 0)
        {
            auto battery_state = rpp_driver_->getBatteryState();
            battery_state_.percentage = battery_state.percentage;
            battery_state_.temperature = battery_state.temperature;
            battery_state_.voltage = battery_state.voltage;
            battery_state_.current = battery_state.current;
            battery_state_.ts = battery_state.ts;
            battery_pub_->publish(battery_state_);
        }

        if (light_pub_->get_subscription_count() != 0)
        {
            auto light_state = rpp_driver_->getLightState();
            light_state_.front_light = light_state.front_light;
            light_state_.rear_light = light_state.rear_light;
            light_state_.ts = light_state.ts;
            light_pub_->publish(light_state_);
        }

        if (error_pub_->get_subscription_count() != 0)
        {
            auto error_state = rpp_driver_->getErrorState();
            error_state_.motor_error = error_state.motor_error;
            error_state_.turn_motor_error = error_state.turn_motor_error;
            error_state_.bump_error = error_state.bump_error;
            error_state_.communication_error = error_state.communication_error;
            error_state_.ts = error_state.ts;
            error_pub_->publish(error_state_);
        }

        if (motor_pub_->get_subscription_count() != 0)
        {
            auto motor_state = rpp_driver_->getMotorState();
            for (size_t i = 0; i < 4; i++)
            {
                motor_state_.motor_states[i].theta = motor_state.motor_states[i].theta;
                motor_state_.motor_states[i].omega = motor_state.motor_states[i].omega;
                motor_state_.motor_states[i].voltage = motor_state.motor_states[i].voltage;
                motor_state_.motor_states[i].current = motor_state.motor_states[i].current;
                motor_state_.motor_states[i].temperature = motor_state.motor_states[i].temperature;
                motor_state_.motor_states[i].rpm = motor_state.motor_states[i].rpm;
                motor_state_.motor_states[i].tick = motor_state.motor_states[i].tick;
                motor_state_.motor_states[i].fault = motor_state.motor_states[i].fault;
            }
            motor_state_.ts = motor_state.ts;
            motor_pub_->publish(motor_state_);
        }

        if (trun_motor_pub_->get_subscription_count() != 0)
        {
            auto trun_motor_state = rpp_driver_->getTurnMotorState();
            for (size_t i = 0; i < 4; i++)
            {
                trun_motor_state_.motor_states[i].theta = trun_motor_state.motor_states[i].theta;
                trun_motor_state_.motor_states[i].omega = trun_motor_state.motor_states[i].omega;
                trun_motor_state_.motor_states[i].voltage = trun_motor_state.motor_states[i].voltage;
                trun_motor_state_.motor_states[i].current = trun_motor_state.motor_states[i].current;
                trun_motor_state_.motor_states[i].temperature = trun_motor_state.motor_states[i].temperature;
                trun_motor_state_.motor_states[i].rpm = trun_motor_state.motor_states[i].rpm;
                trun_motor_state_.motor_states[i].tick = trun_motor_state.motor_states[i].tick;
                trun_motor_state_.motor_states[i].fault = trun_motor_state.motor_states[i].fault;
            }
            trun_motor_state_.ts = trun_motor_state.ts;
            trun_motor_pub_->publish(trun_motor_state_);
        }
    }

    if (twist_pub_->get_subscription_count() != 0)
    {
        auto twist = rpp_driver_->getTwist();
        twist_.linear.x = twist.x;
        twist_.linear.y = twist.y;
        twist_.angular.z = twist.theta;
        twist_pub_->publish(twist_);
    }

    if (publish_odom_)
    {
        auto odom = rpp_driver_->getOdometry();
        odom_.header.stamp = rclcpp::Time(odom.ts);
        odom_.pose.pose.position.x = odom.x;
        odom_.pose.pose.position.y = odom.y;
        odom_.pose.pose.position.z = 0;
        tf2::Quaternion q;
        q.setRPY(0, 0, odom.theta);
        geometry_msgs::msg::Quaternion orientation = tf2::toMsg(q);
        odom_.pose.pose.orientation = orientation;

        if (odom.x == 0 && odom.y == 0 && odom.theta == 0)
        {
            // 如果velocity是零，说明编码器的误差会比较小，认为编码器数据更可靠
            memcpy(&odom_.pose.covariance, odom_pose_covariance2, sizeof(odom_pose_covariance2));
            memcpy(&odom_.twist.covariance, odom_twist_covariance2, sizeof(odom_twist_covariance2));
        }
        else
        {
            // 如果小车velocity非零，考虑到运动中编码器可能带来的滑动误差，认为imu的数据更可靠
            memcpy(&odom_.pose.covariance, odom_pose_covariance, sizeof(odom_pose_covariance));
            memcpy(&odom_.twist.covariance, odom_twist_covariance, sizeof(odom_twist_covariance));
        }
        odom_pub_->publish(odom_);

        if (publish_odom_tf_)
        {
            geometry_msgs::msg::TransformStamped odom_trans;
            odom_trans.header.stamp = odom_.header.stamp;
            odom_trans.header.frame_id = odom_frame_;
            odom_trans.child_frame_id = base_frame_;
            odom_trans.transform.translation.x = odom.x;
            odom_trans.transform.translation.y = odom.y;
            odom_trans.transform.translation.z = 0.0;
            odom_trans.transform.rotation = orientation;
            odom_broadcaster_->sendTransform(odom_trans);
        }
    }
}

} // namespace rpp