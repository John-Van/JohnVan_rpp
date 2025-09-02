# Copyright (c) 2020 Samsung Research Russia
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.conditions import IfCondition, UnlessCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.descriptions import ParameterFile
from nav2_common.launch import HasNodeParams, RewrittenYaml
from launch.substitutions import (
    Command,
    FindExecutable,
    LaunchConfiguration,
    PathJoinSubstitution,
)
from launch_ros.substitutions import FindPackageShare

# 全局变量
slm_2d_path = None
robot_bringup_path = None


def get_default_value_user_config_path():
    global slm_2d_path, robot_bringup_path
    package_name = "slam_2d"
    slm_2d_path = get_package_share_directory(package_name)
    robot_bringup_path = get_package_share_directory("robot_bringup")

    # global_user_config_path = os.path.join(package_path, 'params', 'MID360_config.json')
    # print(f"User config path is: {global_user_config_path}")


def get_nav2_params():

    namespace = DeclareLaunchArgument(
        "namespace",
        default_value="",
        description="每个雷达的命名空间，默认为根命名空间",
    )
    declare_params_fild = DeclareLaunchArgument(
        "params_file",
        default_value=os.path.join(
            robot_bringup_path, "params", "nav2_mppi_params.yaml"
        ),
        description="用于所有启动节点的 ROS2 参数文件的完整路径",
    )
    declare_use_sim_time = DeclareLaunchArgument(
        "use_sim_time",
        default_value="False",
        description="如果为真，则使用模拟（Gazebo）时钟",
    )

    declare_autostart = DeclareLaunchArgument(
        "autostart", default_value="True", description="自动启动 nav2 堆栈"
    )

    use_composition = DeclareLaunchArgument(
        "use_composition", default_value="True", description="是否使用组合启动"
    )

    declare_use_respawn = DeclareLaunchArgument(
        "use_respawn",
        default_value="False",
        description="如果节点崩溃，是否重生。禁用组合时应用。",
    )

    declare_log_level = DeclareLaunchArgument(
        "log_level", default_value="info", description="日志等级"
    )

    use_rviz = DeclareLaunchArgument(
        "use_rviz", default_value="True", description="是否启动 RVIZ"
    )

    use_namespace_cmd = DeclareLaunchArgument(
        "use_namespace",
        default_value="False",
        description="是否在 Navigation stack 应用命名空间",
    )

    map_yaml = DeclareLaunchArgument(
        "map",
        default_value=os.path.join("/home/rpp/rpp_data/map/office.yaml"),
        description="地图的路径",
    )

    rviz_config_file = DeclareLaunchArgument(
        "rviz_config_file",
        default_value=os.path.join(
            robot_bringup_path, "rviz", "nav2_default_view.rviz"
        ),
        description="要使用的 RVIZ 配置文件的完整路径",
    )

    publish_odom = DeclareLaunchArgument(
        "publish_odom", default_value="true", description=""
    )
    publish_odom_tf = DeclareLaunchArgument(
        "publish_odom_tf", default_value="true", description=""
    )

    return [
        namespace,
        declare_params_fild,
        declare_use_sim_time,
        declare_autostart,
        declare_use_respawn,
        declare_log_level,
        use_rviz,
        use_namespace_cmd,
        map_yaml,
        rviz_config_file,
        use_composition,
        publish_odom,
        publish_odom_tf,
    ]


def create_slam_toolbos_nodes(
    use_sim_time, slam_params_file, slam_toolbox_default_params_file
):

    has_slam_toolbox_params = HasNodeParams(
        source_file=slam_params_file, node_name="slam_toolbox"
    )

    start_slam_toolbox_with_default = Node(
        package="slam_toolbox",
        executable="sync_slam_toolbox_node",
        name="slam_toolbox",
        output="screen",
        parameters=[slam_toolbox_default_params_file, {"use_sim_time": use_sim_time}],
        # condition=UnlessCondition(has_slam_toolbox_params)  # 如果未启用自定义参数，则使用默认参数
    )

    start_slam_toolbox_with_custom = Node(
        package="slam_toolbox",
        executable="sync_slam_toolbox_node",
        name="slam_toolbox",
        output="screen",
        parameters=[slam_params_file, {"use_sim_time": use_sim_time}],
        # condition=IfCondition(has_slam_toolbox_params)  # 如果启用自定义参数，则传递参数文件
    )

    if has_slam_toolbox_params:
        return [start_slam_toolbox_with_custom]
    else:
        return [start_slam_toolbox_with_default]


def create_nav2_nodes(
    namespace, use_sim_time, autostart, params_file, use_composition, use_respawn
):

    navigation_node = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [
                PathJoinSubstitution(
                    [
                        FindPackageShare("navigation_2d"),
                        "launch",
                        "navigation.launch.py",
                    ]
                )
            ]
        ),
        launch_arguments={
            "namespace": "",
            "use_sim_time": use_sim_time,
            "autostart": autostart,
            "params_file": params_file,
            "use_composition": use_composition,
            "use_respawn": use_respawn,
            "container_name": "nav2_container",
        }.items(),
    )

    return [navigation_node]


def create_equipment_and_rviz_nodes(
    use_rviz, namespace, use_namespace, rviz_config, publish_odom, publish_odom_tf
):

    lidar_launch_file = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [
                PathJoinSubstitution(
                    [
                        FindPackageShare("robot_bringup"),
                        "launch",
                        "robot_driver.launch.py",
                    ]
                )
            ]
        ),
        launch_arguments={
            "namespace": namespace,
            "use_3d_lidar": "false",
            "use_2d_lidar": "True",
            "publish_odom": publish_odom,
            "publish_odom_tf": publish_odom_tf,
        }.items(),
    )

    rviz_launch_file = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [
                PathJoinSubstitution(
                    [FindPackageShare("robot_bringup"), "launch", "rviz.launch.py"]
                )
            ]
        ),
        launch_arguments={
            "namespace": namespace,
            "use_namespace": use_namespace,
            "rviz_config": rviz_config,
        }.items(),
        condition=IfCondition(use_rviz),
    )

    return [lidar_launch_file, rviz_launch_file]


def generate_launch_description():
    get_default_value_user_config_path()
    params_ = get_nav2_params()

    # Input parameters declaration
    namespace = LaunchConfiguration("namespace")
    params_file = LaunchConfiguration("params_file")
    use_sim_time = LaunchConfiguration("use_sim_time")
    autostart = LaunchConfiguration("autostart")
    use_respawn = LaunchConfiguration("use_respawn")
    log_level = LaunchConfiguration("log_level")
    use_rviz = LaunchConfiguration("use_rviz")
    use_namespace = LaunchConfiguration("use_namespace")
    rviz_config = LaunchConfiguration("rviz_config_file")
    use_composition = LaunchConfiguration("use_composition")
    publish_odom = LaunchConfiguration("publish_odom")
    publish_odom_tf = LaunchConfiguration("publish_odom_tf")
    # use_composition= LaunchConfiguration('rviz_config_file')
    # Variables
    lifecycle_nodes = ["map_saver"]
    param_substitutions = {"use_sim_time": use_sim_time}

    configured_params = ParameterFile(
        RewrittenYaml(
            source_file=params_file,
            root_key=namespace,
            param_rewrites=param_substitutions,
            convert_types=True,
        ),
        allow_substs=True,
    )

    nav2_nodes = create_nav2_nodes(
        namespace=namespace,
        use_sim_time=use_sim_time,
        autostart=autostart,
        params_file=params_file,
        use_composition=use_composition,
        use_respawn=use_respawn,
    )

    slam_toolbos_nodes = create_slam_toolbos_nodes(
        use_sim_time=use_sim_time,
        slam_params_file=params_file,
        slam_toolbox_default_params_file=os.path.join(
            slm_2d_path, "config", "mapper_params_online_sync.yaml"
        ),
    )
    robot = create_equipment_and_rviz_nodes(
        use_rviz,
        namespace,
        use_namespace,
        rviz_config,
        publish_odom,
        publish_odom_tf,
    )

    ld = LaunchDescription()
    for node in params_ + slam_toolbos_nodes + robot:
        ld.add_action(node)

    return ld
