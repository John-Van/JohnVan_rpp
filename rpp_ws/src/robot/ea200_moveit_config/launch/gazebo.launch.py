from launch import LaunchDescription, LaunchContext
from launch.substitutions import Command, FindExecutable, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch.actions import IncludeLaunchDescription, RegisterEventHandler
from launch.event_handlers import OnProcessExit
from launch.launch_description_sources import PythonLaunchDescriptionSource


def generate_launch_description():

    robot_description = Command(
        [
            PathJoinSubstitution([FindExecutable(name="xacro")]),
            " ",
            PathJoinSubstitution([FindPackageShare("ea200_description"), "xacro", "ea200.xacro"]),
            " ",
            "sim_gazebo:=",
            "true",
        ]
    ).perform(LaunchContext())

    rviz_config_file = PathJoinSubstitution([FindPackageShare("ea200_description"), "config", "moveit.rviz"])

    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([PathJoinSubstitution([FindPackageShare("gazebo_ros"), "launch", "gazebo.launch.py"])]),
        launch_arguments={"verbose": "false"}.items(),
    )
    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        output="screen",
        parameters=[{"robot_description": robot_description}],
    )
    spawn_entity = Node(
        package="gazebo_ros",
        executable="spawn_entity.py",
        name="spawn_robot",
        arguments=["-topic", "robot_description", "-entity", "ea200", "-z", "0.05"],
        output="screen",
    )
    load_joint_broadcaster = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster", "-c", "/controller_manager"],
    )
    load_diff_drive_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["diff_drive_base_controller", "-c", "/controller_manager"],
    )
    load_body_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["body_controller", "-c", "/controller_manager"],
    )
    load_arm_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["arm_controller", "-c", "/controller_manager"],
    )
    load_left_gripper_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["left_gripper_controller", "-c", "/controller_manager"],
    )
    load_right_gripper_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["right_gripper_controller", "-c", "/controller_manager"],
    )
    load_controllers = RegisterEventHandler(
        event_handler=OnProcessExit(
            target_action=spawn_entity,
            on_exit=[
                load_joint_broadcaster,
                load_diff_drive_controller,
                load_body_controller,
                load_arm_controller,
                load_left_gripper_controller,
                load_right_gripper_controller,
            ],
        )
    )
    load_rviz2 = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        arguments=["-d", rviz_config_file],
    )

    return LaunchDescription(
        [
            robot_state_publisher,
            gazebo,
            spawn_entity,
            # load_controllers,
            # RegisterEventHandler(event_handler=OnProcessExit(target_action=load_joint_broadcaster, on_exit=[load_rviz2])),
        ]
    )
