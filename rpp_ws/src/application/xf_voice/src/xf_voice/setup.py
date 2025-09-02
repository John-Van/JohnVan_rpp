# 在xf_voice功能包的setup.py中添加以下内容（在entry_points部分）
entry_points={
    'console_scripts': [
        # 已有的节点...
        'voice_dialog = xf_voice.voice_dialog_node:main',
    ],
},
    