# Copyright (c) 2018 Intel Corporation
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
from launch.actions import DeclareLaunchArgument, GroupAction, SetEnvironmentVariable
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch_ros.actions import LoadComposableNodes
from launch_ros.actions import Node
from launch_ros.descriptions import ComposableNode, ParameterFile
from nav2_common.launch import RewrittenYaml

slm_2d_path = None
robot_bringup_path = None


def get_default_value_user_config_path():
    global slm_2d_path, robot_bringup_path
    package_name = "slam_2d"
    slm_2d_path = get_package_share_directory(package_name)
    robot_bringup_path = get_package_share_directory("robot_bringup")


def get_nav2_params():

    namespace = DeclareLaunchArgument(
        "namespace",
        default_value="",
        description="每个雷达的命名空间，默认为根命名空间",
    )

    map_yaml = DeclareLaunchArgument(
        "map",
        default_value=os.path.join(
            os.environ["HOME"], "rpp_data/map/slam_toolbox/map.yaml"
        ),  # todo:可能有问题
        description="地图的路径",
    )

    declare_use_sim_time = DeclareLaunchArgument(
        "use_sim_time",
        default_value="False",
        description="如果为真，则使用模拟（Gazebo）时钟",
    )

    declare_params_fild = DeclareLaunchArgument(
        "params_file",
        default_value=os.path.join(
            robot_bringup_path, "params", "nav2_mppi_params.yaml"
        ),
        description="用于所有启动节点的 ROS2 参数文件的完整路径",
    )

    declare_autostart = DeclareLaunchArgument(
        "autostart", default_value="True", description="自动启动 nav2 堆栈"
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
        default_value="false",
        description="是否在 Navigation stack 应用命名空间",
    )

    lio_file = DeclareLaunchArgument(
        "lio_config_file",
        default_value=os.path.join(robot_bringup_path, "params", "lio.yaml"),
        description="fast lio sam 配置文件",
    )

    localize_config_file = DeclareLaunchArgument(
        "localize_config_file",
        default_value=os.path.join(robot_bringup_path, "params", "localizer.yaml"),
        description="fast lio sam  localize配置文件",
    )

    use_composition = DeclareLaunchArgument(
        "use_composition", default_value="False", description="是否使用组合启动"
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
        lio_file,
        localize_config_file,
        use_composition,
    ]


def create_localizer_nodes(
    namespace,
    use_respawn,
    log_level,
    configured_params,
    autostart,
    lifecycle_nodes,
    use_sim_time,
    remappings,
):
    map_saver_server_node = Node(
        package="nav2_map_server",
        executable="map_server",
        name="map_server",
        namespace=namespace,
        respawn=use_respawn,
        respawn_delay=2.0,
        arguments=["--ros-args", "--log-level", log_level],
        parameters=[configured_params],
        output="screen",
    )

    amcl_localizer_nodes = Node(
        package="nav2_amcl",
        executable="amcl",
        name="amcl",
        output="screen",
        respawn=use_respawn,
        respawn_delay=2.0,
        parameters=[configured_params],
        arguments=["--ros-args", "--log-level", log_level],
        remappings=remappings,
    )

    nav2_lifecycle_manager_node = Node(
        package="nav2_lifecycle_manager",
        executable="lifecycle_manager",
        name="lifecycle_manager_navigation",
        output="screen",
        arguments=["--ros-args", "--log-level", log_level],
        parameters=[
            {"use_sim_time": use_sim_time},
            {"autostart": autostart},
            {"node_names": lifecycle_nodes},
        ],
    )
    return [map_saver_server_node, amcl_localizer_nodes, nav2_lifecycle_manager_node]


def create_group_action_nodes(
    use_composition,
    namespace,
    use_respawn,
    log_level,
    configured_params,
    autostart,
    lifecycle_nodes,
    use_sim_time,
    remappings,
):
    localizer_nodes = create_localizer_nodes(
        namespace=namespace,
        use_respawn=use_respawn,
        log_level=log_level,
        configured_params=configured_params,
        autostart=autostart,
        lifecycle_nodes=lifecycle_nodes,
        use_sim_time=use_sim_time,
        remappings=remappings,  # 1
    )  #
    load_nodes = GroupAction(
        condition=IfCondition(PythonExpression(["not ", use_composition])),
        actions=localizer_nodes,
    )
    return [load_nodes]


def create_composable_nodes(
    remappings, configured_params, autostart, lifecycle_nodes, use_sim_time
):
    map_server_composable_nodes = ComposableNode(
        package="nav2_map_server",
        plugin="nav2_map_server::MapServer",
        name="map_server",
        parameters=[configured_params],
        remappings=remappings,
    )

    amcl_composable_nodes = ComposableNode(
        package="nav2_amcl",
        plugin="nav2_amcl::AmclNode",
        name="amcl",
        parameters=[configured_params],
        remappings=remappings,
    )

    lifecycle_manager_localization_composable_nodes = ComposableNode(
        package="nav2_lifecycle_manager",
        plugin="nav2_lifecycle_manager::LifecycleManager",
        name="lifecycle_manager_localization",
        parameters=[
            {
                "use_sim_time": use_sim_time,
                "autostart": autostart,
                "node_names": lifecycle_nodes,
            }
        ],
    )
    return [
        map_server_composable_nodes,
        amcl_composable_nodes,
        lifecycle_manager_localization_composable_nodes,
    ]


def load_composable_nodes(
    use_composition,
    remappings,
    configured_params,
    autostart,
    lifecycle_nodes,
    use_sim_time,
    container_name_full,
):
    composable_nodes = create_composable_nodes(
        remappings=remappings,
        configured_params=configured_params,
        autostart=autostart,
        lifecycle_nodes=lifecycle_nodes,
        use_sim_time=use_sim_time,
    )
    load_composable_nodes = LoadComposableNodes(
        condition=IfCondition(use_composition),
        target_container=container_name_full,
        composable_node_descriptions=composable_nodes,
    )
    return [load_composable_nodes]


def generate_launch_description():

    get_default_value_user_config_path()
    nav2_params = get_nav2_params()
    namespace = LaunchConfiguration("namespace")
    use_sim_time = LaunchConfiguration("use_sim_time")
    autostart = LaunchConfiguration("autostart")
    params_file = LaunchConfiguration("params_file")
    use_composition = LaunchConfiguration("use_composition")
    container_name = LaunchConfiguration("container_name")
    container_name_full = (namespace, "/", container_name)
    use_respawn = LaunchConfiguration("use_respawn")
    log_level = LaunchConfiguration("log_level")

    localizer_config_path = LaunchConfiguration("localize_config_file")
    map_yaml_file = LaunchConfiguration("map")

    lifecycle_nodes = ["map_server", "amcl"]
    # Map fully qualified names to relative ones so the node's namespace can be prepended.
    # In case of the transforms (tf), currently, there doesn't seem to be a better alternative
    # https://github.com/ros/geometry2/issues/32
    # https://github.com/ros/robot_state_publisher/pull/30
    # TODO(orduno) Substitute with `PushNodeRemapping`
    #              https://github.com/ros2/launch_ros/issues/56
    remappings = [("/tf", "tf"), ("/tf_static", "tf_static")]
    # Create our own temporary YAML files that include substitutions
    param_substitutions = {"use_sim_time": use_sim_time, "yaml_filename": map_yaml_file}

    configured_params = ParameterFile(
        RewrittenYaml(
            source_file=params_file,
            root_key=namespace,
            param_rewrites=param_substitutions,
            convert_types=True,
        ),
        allow_substs=True,
    )

    print(configured_params)
    action_nodes = create_group_action_nodes(
        use_composition=use_composition,
        remappings=remappings,
        namespace=namespace,
        use_respawn=use_respawn,
        log_level=log_level,
        configured_params=configured_params,
        autostart=autostart,
        lifecycle_nodes=lifecycle_nodes,
        use_sim_time=use_sim_time,
    )  #

    composable_nodes = load_composable_nodes(
        use_composition=use_composition,
        remappings=remappings,
        configured_params=configured_params,
        autostart=autostart,
        lifecycle_nodes=lifecycle_nodes,
        use_sim_time=use_sim_time,
        container_name_full=container_name_full,
    )

    stdout_linebuf_envvar = [
        SetEnvironmentVariable("RCUTILS_LOGGING_BUFFERED_STREAM", "1")
    ]
    ld = LaunchDescription()

    for node in nav2_params + action_nodes + composable_nodes + stdout_linebuf_envvar:
        ld.add_action(node)
    return ld
