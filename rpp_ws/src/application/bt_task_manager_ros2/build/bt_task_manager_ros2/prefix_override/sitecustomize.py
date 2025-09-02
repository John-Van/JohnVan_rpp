import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/rpp/rpp_ws/src/application/bt_task_manager_ros2/install/bt_task_manager_ros2'
