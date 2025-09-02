
#include <rclcpp/rclcpp.hpp>
#include "rpp_ros_driver/rpp_ros_driver.h"

std::shared_ptr<rpp::RPPRosDriver> driver = nullptr;

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    try
    {
        driver = std::make_shared<rpp::RPPRosDriver>();
    }
    catch (const std::exception &e)
    {
        RCLCPP_ERROR(driver->get_logger(), "Failed to construct RobusterDriver object: %s", e.what());
    }

    rclcpp::spin(driver);
    driver.reset();
    rclcpp::shutdown();
    return 0;
}