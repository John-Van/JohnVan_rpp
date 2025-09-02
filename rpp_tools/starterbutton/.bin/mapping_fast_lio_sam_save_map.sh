#! /bin/bash
source ~/.rpprc
ros2 service call /pgo/save_maps interface/srv/SaveMaps "{file_path: '$HOME/rpp_data/map/fast_lio_sam', save_patches: true}"
# rosservice call /save_map "resolution: 0.0
# destination: '/rpp_data/map/fast_lio_sam'" 
