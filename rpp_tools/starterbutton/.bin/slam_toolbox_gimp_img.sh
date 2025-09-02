#! /bin/bash
source ~/.rpprc
pcd_path="$HOME/rpp_data/map/slam_toolbox/map.pcd"
#pcd_path="$HOME/rpp_data/map/slam_toolbox/filterGlobalMap.pcd"
out_path="$HOME/rpp_data/map/slam_toolbox"
#cp $pcd_path  $out_path #/GlobalMap.pcd
gimp  $out_path/map.pgm
