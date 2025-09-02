#! /bin/bash
source ~/.rpprc
ros2 topic pub /grasp_node/cmd std_msgs/msg/String "data: 'place'"  -1
