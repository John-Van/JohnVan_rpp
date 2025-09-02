import launch
import launch_ros.actions
from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

rviz_cfg = None
pgo_config_path = None
lio_config_path = None


def get_default_value_user_config_path():
    global rviz_cfg, pgo_config_path, lio_config_path
    rviz_cfg = PathJoinSubstitution([FindPackageShare("slam_3d"), "rviz", "pgo.rviz"])
    pgo_config_path = PathJoinSubstitution(
        [FindPackageShare("slam_3d"), "config", "pgo.yaml"]
    )

    lio_config_path = PathJoinSubstitution(
        [FindPackageShare("slam_3d"), "config", "lio.yaml"]
    )


def create_fast_lio_nodes():

    # rviz_cfg = PathJoinSubstitution(
    #     [FindPackageShare("slam_3d"), "rviz", "pgo.rviz"]
    # )
    # pgo_config_path = PathJoinSubstitution(
    #     [FindPackageShare("slam_3d"), "config", "pgo.yaml"]
    # )

    # lio_config_path = PathJoinSubstitution(
    #     [FindPackageShare("slam_3d"), "config", "lio.yaml"]
    # )

    lio_node = Node(
        package="fastlio2",
        namespace="fastlio2",
        executable="lio_node",
        name="lio_node",
        output="screen",
        parameters=[{"config_path": lio_config_path.perform(launch.LaunchContext())}],
    )
    pgo_node = Node(
        package="pgo",
        namespace="pgo",
        executable="pgo_node",
        name="pgo_node",
        output="screen",
        parameters=[{"config_path": pgo_config_path.perform(launch.LaunchContext())}],
    )
    pgo_rviz_node = Node(
        package="rviz2",
        namespace="pgo",
        executable="rviz2",
        name="rviz2",
        output="screen",
        arguments=["-d", rviz_cfg.perform(launch.LaunchContext())],
    )
    return [lio_node, pgo_node, pgo_rviz_node]


def create_equipment_nodes():
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
            "namespace": "/",
            "use_3d_lidar": "True",
            "use_2d_lidar": "false",
        }.items(),
    )
    return [lidar_launch_file]


def generate_launch_description():
    get_default_value_user_config_path()
    fast_lio_nodes = create_fast_lio_nodes()
    robot = create_equipment_nodes()
    ld = LaunchDescription()
    for node in fast_lio_nodes + robot:
        ld.add_action(node)
    return ld
