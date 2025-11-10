#!/usr/bin/env python3 
# -*- coding: utf-8 -*-
import os
from glob import glob
from setuptools import find_packages, setup

package_name = 'linker_hand_ros2_sdk'

this_dir = os.path.abspath(os.path.dirname(__file__))  # setup.py所在目录
custom_dir = os.path.join(this_dir, package_name, "LinkerHand")  # LinkerHand根目录

data_files = [
    ('share/ament_index/resource_index/packages',
     ['resource/' + package_name]),
    ('share/' + package_name, ['package.xml']),
    (os.path.join('share', package_name, 'launch'), glob('launch/*.launch.py')),
]

# 关键：添加LinkerHand目录下的所有文件（包括config/setting.yaml）
for root, dirs, files in os.walk(custom_dir):
    if files:
        # 计算当前目录相对于package_name的相对路径（用于安装路径）
        relative_path = os.path.relpath(root, os.path.join(this_dir, package_name))
        # 安装目标路径：对应到lib/{package_name}/下的相对路径
        target_path = os.path.join('lib', package_name, relative_path)
        # 源文件路径：必须是相对于setup.py所在目录的相对路径
        files_relative = [os.path.relpath(os.path.join(root, f), start=this_dir) for f in files]
        data_files.append((target_path, files_relative))

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(include=[package_name, f"{package_name}.*"]),
    data_files=data_files,
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='linker-robot',
    maintainer_email='linker-robot@todo.todo',
    description='ROS2 SDK for Linker Hand',
    license='TODO: License declaration',
    entry_points={
        'console_scripts': [
            'linker_hand_sdk = linker_hand_ros2_sdk.linker_hand:main',
        ],
    },
)