#! /bin/bash
source ~/.rpprc
source starterbutton/.bin/com.sh
SLAM_2D_BUILD_MAP_PLAN="slam_toolbox"
ros2 launch slam_2d slam_toolbox_mapping.launch.py 

