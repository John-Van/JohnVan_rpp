#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>
#include <std_msgs/msg/header.hpp>
#include <livox_ros_driver2/msg/custom_msg.hpp>
class LidarClipping : public rclcpp::Node
{
public:
    LidarClipping() : Node("lidar_clipping")
    {
        this->declare_parameter("min_x", -0.65);
        this->declare_parameter("max_x", 0.65);
        this->declare_parameter("min_y", -0.35);
        this->declare_parameter("max_y", 0.35);
        this->declare_parameter("min_z", -0.2);
        this->declare_parameter("max_z", 0.8);
        this->declare_parameter("point_cloud_topic", "point_cloud_topic");
        this->declare_parameter("filtered_point_cloud_topic", "filtered_point_cloud_topic");
        this->declare_parameter("lidar_type", "livox"); 

        // 设定裁剪的范围参数
        min_x_ = this->get_parameter("min_x").as_double();
        max_x_ = this->get_parameter("max_x").as_double();
        min_y_ = this->get_parameter("min_y").as_double();
        max_y_ = this->get_parameter("max_y").as_double();
        min_z_ = this->get_parameter("min_z").as_double();
        max_z_ = this->get_parameter("max_z").as_double();
        lidar_type_ = this->get_parameter("lidar_type").as_string();
        point_cloud_topic_ = this->get_parameter("point_cloud_topic").as_string();
        filtered_point_cloud_topic_ = this->get_parameter("filtered_point_cloud_topic").as_string();
        if (lidar_type_ == "livox")
        {
            subscription_ = this->create_subscription<livox_ros_driver2::msg::CustomMsg>(
                point_cloud_topic_, 10, std::bind(&LidarClipping::livoxCallback, this, std::placeholders::_1));
            filtered_pub_ = this->create_publisher<livox_ros_driver2::msg::CustomMsg>(filtered_point_cloud_topic_, 10);
        }
        else if (lidar_type_ == "velodyne")
        {
            pointcloud_subscription_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
                point_cloud_topic_, 10, std::bind(&LidarClipping::pointCloudCallback, this, std::placeholders::_1));
            pointcloud_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>(filtered_point_cloud_topic_, 10);
        }

    }

private:
    void pointCloudCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
    {
        // 创建一个新的 PointCloud2 消息用于发布
        sensor_msgs::msg::PointCloud2 filtered_msg = *msg;

        // 获取点云数据的原始字节
        uint8_t* data_ptr = msg->data.data();

        // 获取点云每个点的字节数
        size_t point_step = msg->point_step;  // 每个点的字节数

        // 新的点云数据容器
        std::vector<uint8_t> filtered_data;

        // 遍历所有点
        size_t num_points = msg->width * msg->height;
        for (size_t i = 0; i < num_points; ++i)
        {
            // 计算当前点的位置
            uint8_t* point_ptr = data_ptr + i * point_step;

            // 提取当前点的坐标 (x, y, z)
            float x = *reinterpret_cast<float*>(point_ptr);
            float y = *reinterpret_cast<float*>(point_ptr + sizeof(float));
            float z = *reinterpret_cast<float*>(point_ptr + 2 * sizeof(float));

            // 判断点是否在范围外
            if (x < min_x_ || x > max_x_ || y < min_y_ || y > max_y_ || z < min_z_ || z > max_z_)
            {
                // 如果在范围外，将该点添加到过滤后的数据中
                filtered_data.insert(filtered_data.end(), point_ptr, point_ptr + point_step);
            }
        }

        // 设置过滤后的点云数据
        filtered_msg.data = filtered_data;
        filtered_msg.width = filtered_data.size() / msg->point_step;  // 更新点云的宽度
        filtered_msg.height = 1;  // 点云是无结构的，设置为 1

        // 发布过滤后的点云
        pointcloud_pub_->publish(filtered_msg);
    }
    void livoxCallback(const livox_ros_driver2::msg::CustomMsg::SharedPtr msg)
    {
        // 遍历所有点
        size_t num_points = msg->point_num;
        for (size_t i = 0; i < num_points; i++)
        {
            if ((msg->points[i].line < 4) && ((msg->points[i].tag & 0x30) == 0x10 || (msg->points[i].tag & 0x30) == 0x00))
            {


            // 提取当前点的坐标 (x, y, z)
            float x = msg->points[i].x;
            float y = msg->points[i].y;
            float z = msg->points[i].z;

            // 判断点是否在范围外
            if (x < min_x_ || x > max_x_ || y < min_y_ || y > max_y_ || z < min_z_ || z > max_z_)
            {
                continue;
            }
            else
            {
                msg->points[i].x = NAN;
                msg->points[i].y = NAN;
                msg->points[i].z = NAN;
            }
            
        }
        }
        // 发布过滤后的点云
        livox_ros_driver2::msg::CustomMsg filtered_msg = *msg;
        filtered_pub_->publish(filtered_msg);

    }
    rclcpp::Subscription<livox_ros_driver2::msg::CustomMsg>::SharedPtr subscription_;
    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr pointcloud_subscription_;
    rclcpp::Publisher<livox_ros_driver2::msg::CustomMsg>::SharedPtr filtered_pub_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pointcloud_pub_;
    double min_x_, max_x_, min_y_, max_y_, min_z_, max_z_;
    std::string point_cloud_topic_, filtered_point_cloud_topic_, lidar_type_; 
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LidarClipping>());
    rclcpp::shutdown();
    return 0;
}
