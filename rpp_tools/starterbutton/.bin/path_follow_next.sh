#! /bin/bash

source ~/.rpprc
rosservice call /waypoint_follow "control: 'next'"