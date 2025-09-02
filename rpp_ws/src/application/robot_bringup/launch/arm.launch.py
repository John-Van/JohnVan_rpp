from launch import LaunchDescription, LaunchContext
from launch.actions import RegisterEventHandler, DeclareLaunchArgument, OpaqueFunction
from launch.conditions import IfCondition
from launch.event_handlers import OnProcessExit
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution, PythonExpression
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def launch_setup(context, *args, **kwargs):
    robot_controllers = PathJoinSubstitution([FindPackageShare("robot_bringup"), "params", "arm_controllers.yaml"])

    control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[robot_controllers],
        remappings=[
            ("~/robot_description", "/robot_description"),
        ],
        output="both",
    )

    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=[
            "joint_state_broadcaster",
            "--controller-manager",
            "controller_manager",
        ],
    )

    load_left_joint_trajectory_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["left_joint_trajectory_controller", "-c", "controller_manager"],
    )

    load_right_joint_trajectory_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["right_joint_trajectory_controller", "-c", "controller_manager"],
    )


    fault_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["fault_controller", "-c", "controller_manager"],
    )

    nodes_to_start = [
        control_node,
        joint_state_broadcaster_spawner,
        load_left_joint_trajectory_controller,
        load_right_joint_trajectory_controller,
        fault_controller_spawner,
    ]

    return nodes_to_start


def generate_launch_description():
    return LaunchDescription([OpaqueFunction(function=launch_setup)])