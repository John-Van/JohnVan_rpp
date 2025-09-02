#include <rclcpp/rclcpp.hpp>
#include "motor_lift_ros/motor_lift_ros.h"

std::shared_ptr<rpp::Syntron_Servo_ROS> driver = nullptr;

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);

  try
  {
    driver = std::make_shared<rpp::Syntron_Servo_ROS>();
  }
  catch (const std::exception& e)
  {
    RCLCPP_ERROR(driver->get_logger(), "Failed to construct RobusterDriver object: %s", e.what());
  }

  rclcpp::spin(driver);
  return 0;
}