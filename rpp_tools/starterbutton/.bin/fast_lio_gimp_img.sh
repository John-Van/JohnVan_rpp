#! /bin/bash
source ~/.rpprc
pcd_path="$HOME/rpp_data/map/fast_lio_sam/map.pcd"
#pcd_path="$HOME/rpp_data/map/fast_lio_sam/filterGlobalMap.pcd"
out_path="$HOME/rpp_data/map/fast_lio_sam"
#cp $pcd_path  $out_path #/GlobalMap.pcd
gimp  $out_path/map.png
