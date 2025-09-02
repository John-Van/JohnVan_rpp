# voice_system_launch.py
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, GroupAction
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():

    # ───────────── YAML 配置（TTS 专用） ─────────────
    tts_yaml = os.path.join(
        get_package_share_directory("xf_voice"), "config", "tts_params.yaml"
    )

    # ───────────── Launch 参数声明（两节点公共 & 快捷项） ─────────────
    declared_arguments = [
        DeclareLaunchArgument("package_path", default_value="/home/"),
        DeclareLaunchArgument("iat_appid", default_value="9a7a7ebd"),
        DeclareLaunchArgument("iat_maxTime", default_value="15"),
        DeclareLaunchArgument("tts_appid", default_value="9a7a7ebd"),
        # 允许从命令行快速开关播放功能（覆盖 YAML）
        DeclareLaunchArgument("play_after_tts", default_value="true"),
    ]

    # ───────────── IAT 节点 ─────────────
    iat_node = Node(
        package="xf_voice",
        executable="iat_publish",
        name="iat_node",
        output="screen",
                    remappings=[
                ("/awake_flag", "/awake_voixe_control_flag")
            ],
        parameters=[
            {
                "iat_appid": LaunchConfiguration("iat_appid"),
                "maxTime": LaunchConfiguration("iat_maxTime"),
                "package_path": LaunchConfiguration("package_path"),
            }
        ],
    )

    # ───────────── TTS 节点 ─────────────
    #   先加载 YAML，再用 LaunchConfiguration 覆盖需要动态改的键
    tts_node = Node(
        package="xf_voice",
        executable="tts_subscribe",
        name="tts_node",
        output="screen",
        parameters=[
            tts_yaml,
            {  # 运行时可覆盖的键
                "tts_appid": LaunchConfiguration("tts_appid"),
                "play_after_tts": LaunchConfiguration("play_after_tts"),
            },
        ],
    )
    # ───────────── 六麦驱动─────────────
    #   先加载 YAML，再用 LaunchConfiguration 覆盖需要动态改的键
    wheeltec_mic = Node(
        package="wheeltec_mic_ros2",
        executable="wheeltec_mic",
        output="screen",
        parameters=[
            {"usart_port_name": "/dev/wheeltec_mic", "serial_baud_rate": 115200}
        ],
    )
    # ───────────── 六麦语音唤醒─────────────
    voice_control = Node(
        package="wheeltec_mic_ros2",
        executable="voice_control",
        output="screen",
        parameters=[{"appid": "12ed41c0"}],  # 独立
    )
    voice_control_ = Node(
        package="xf_voice",
        executable="voice_control",
        output="screen",
    )

    # ───────────── Launch 描述返回 ─────────────
    return LaunchDescription(declared_arguments + [iat_node, tts_node,wheeltec_mic,voice_control,voice_control_])
