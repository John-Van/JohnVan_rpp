#! /bin/bash
source ~/.rpprc
ros2 topic pub /controller_node/dashboard/cancel std_msgs/Empty "{}" -1
