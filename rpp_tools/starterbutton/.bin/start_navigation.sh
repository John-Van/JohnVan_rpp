#! /bin/bash
source ~/.rpprc
source starterbutton/.bin/com.sh
LOCALIZATION=$(read_key_value "SLAM_PLAN" "$HOME/rpp_data/config/rpp/SLAM_PLAN.config")
use_localization="NONE"
function localization_plan() {
    case "$LOCALIZATION" in
        "fast_lio_sam")
            echo "Running fast_lio_sam localization."
            use_localization=fast_lio_sam
            ;;
        "slam_toolbox")
            echo "Running amcl localization."
            use_localization="amcl"
            #echo "Running aslam_toolbox localization."
            ;;
        *)
            echo "Unknown SLAM plan: $1"
            ;;
    esac
}
localization_plan

roslaunch navigation_2d navigation.launch use_localization:=$use_localization