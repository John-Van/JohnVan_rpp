from launch import LaunchDescription, LaunchContext
from launch.actions import IncludeLaunchDescription, RegisterEventHandler, DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.event_handlers import OnProcessExit
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, FindExecutable, PathJoinSubstitution, PythonExpression, LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    declared_arguments = []

    declared_arguments.append(
        DeclareLaunchArgument(
            "launch_rviz",
            default_value="true",
            description="Launch RViz?",
        )
    )

    declared_arguments.append(
        DeclareLaunchArgument(
            "arm_control_mode",
            default_value="position",
            choices=["position", "trajectory"],
            description="arm_control_mode",
        )
    )
    launch_rviz = LaunchConfiguration("launch_rviz")
    arm_control_mode = LaunchConfiguration("arm_control_mode")

    robot_controllers = PathJoinSubstitution([FindPackageShare("ea200_description"), "config", "ros2_controllers.yaml"])

    robot_description = Command(
        [
            PathJoinSubstitution([FindExecutable(name="xacro")]),
            " ",
            PathJoinSubstitution([FindPackageShare("ea200_description"), "xacro", "ea200.xacro"]),
            " ",
            "sim_gazebo:=",
            "true",
            " ",
            "simulation_controllers:=",
            robot_controllers,
        ]
    ).perform(LaunchContext())

    rviz_config_file = PathJoinSubstitution([FindPackageShare("ea200_description"), "rviz", "view_robot.rviz"])

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
        arguments=["-topic", "robot_description", "-entity", "ea200", "-z", "0.08"],
        output="screen",
    )
    load_joint_broadcaster = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster", "-c", "controller_manager"],
    )
    load_diff_drive_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["diff_drive_base_controller", "-c", "controller_manager"],
    )
    load_body_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["body_controller", "-c", "controller_manager"],
    )
    load_joint_position_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_position_controller", "-c", "controller_manager"],
        condition=IfCondition(PythonExpression(["'", arm_control_mode, "' == 'position'"])),
    )
    load_joint_trajectory_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_trajectory_controller", "-c", "controller_manager"],
        condition=IfCondition(PythonExpression(["'", arm_control_mode, "' == 'trajectory'"])),
    )
    load_controllers = RegisterEventHandler(
        event_handler=OnProcessExit(
            target_action=spawn_entity,
            on_exit=[
                load_joint_broadcaster,
                load_diff_drive_controller,
                load_body_controller,
                load_joint_position_controller,
                load_joint_trajectory_controller,
            ],
        )
    )
    load_rviz2 = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        arguments=["-d", rviz_config_file],
        condition=IfCondition(launch_rviz),
    )

    return LaunchDescription(
        declared_arguments
        + [
            robot_state_publisher,
            gazebo,
            spawn_entity,
            load_controllers,
            RegisterEventHandler(event_handler=OnProcessExit(target_action=load_joint_broadcaster, on_exit=[load_rviz2])),
        ]
    )
