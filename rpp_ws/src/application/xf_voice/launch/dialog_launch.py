from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        # 启动语音识别节点
        Node(
            package='xf_voice',
            executable='iat_publish',
            name='iat_node'
        ),
        # 启动TTS节点
        Node(
            package='xf_voice',
            executable='tts_subscribe',
            name='tts_node'
        ),
        # 启动语音唤醒节点
        Node(
            package='xf_voice',
            executable='voice_control',
            name='voice_control_node'
        ),
        # 启动新的对话节点
        Node(
            package='xf_voice',
            executable='voice_dialog',
            name='voice_dialog_node',
            parameters=[
                {'app_id': '你的APPID'},
                {'api_key': '你的APIKey'},
                {'api_secret': '你的APISecret'}
            ]
        )
    ])
    