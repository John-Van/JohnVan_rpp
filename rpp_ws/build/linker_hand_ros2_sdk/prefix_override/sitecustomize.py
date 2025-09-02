import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/rpp/rpp_ws/install/linker_hand_ros2_sdk'
