#! /bin/bash
source ~/.rpprc
pcd_path="$HOME/rpp_data/map/fast_lio_sam/map.pcd"
#pcd_path="$HOME/rpp_data/map/fast_lio_sam/filterGlobalMap.pcd"
out_path="$HOME/rpp_data/map/fast_lio_sam"
#cp $pcd_path  $out_path #/GlobalMap.pcd
pointcloud_to_2dmap --resolution 0.05 --min_points_in_pix 3 --max_points_in_pix 10 --min_height 0.2 --max_height 1 $pcd_path  $out_path
