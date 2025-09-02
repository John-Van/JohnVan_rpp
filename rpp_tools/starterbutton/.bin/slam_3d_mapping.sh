#! /bin/bash
source ~/.rpprc
source starterbutton/.bin/com.sh
SLAM_3D_BUILD_MAP_PLAN="fast_lio_sam"
write_key_value "SLAM_PLAN" "$SLAM_3D_BUILD_MAP_PLAN"  "$HOME/rpp_data/config/rpp/SLAM_PLAN.config"
ros2 launch slam_3d pgo.launch.py 
