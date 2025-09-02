#! /bin/bash

source ~/.rpprc
rosservice call /save_map "resolution: 0.0 
destination: ''" 
