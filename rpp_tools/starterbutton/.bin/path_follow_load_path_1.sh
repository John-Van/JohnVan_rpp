#! /bin/bash

source ~/.rpprc

rosparam set /waypoint_follow/path_file "/home/rpp/paths/path_1.json"
rosparam set /waypoint_follow/waypoint_file  "/home/rpp/paths/path_waypoints_1.json"
sleep 1
rosservice call /waypoint_follow "control: 'reload'"