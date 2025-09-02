import os
from glob import glob
from setuptools import find_packages, setup

package_name = "bt_task_manager_ros2"

setup(
    name=package_name,
    version="0.0.0",
    packages=find_packages(include=[package_name], exclude=["test"]),
    data_files=[
        ("share/" + package_name, ["package.xml"]),
        ("share/ament_index/resource_index/packages", ["resource/" + package_name]),
        (os.path.join("share", package_name, "launch"), glob("launch/*.py")),
        (os.path.join("share", package_name, "config"), glob("config/*.yaml")),
    ],
    install_requires=["setuptools"],
    zip_safe=True,
    maintainer="sjl",
    maintainer_email="shaojiale@robotplusplus.com.cn",
    description="TODO: Package description",
    license="TODO: License declaration",
    tests_require=["pytest"],
    entry_points={
        "console_scripts": [
            "chassis = bt_task_manager_ros2.examples.ex_chassis_move:main",
            "navigation = bt_task_manager_ros2.examples.ex_navigation:main",
            "arm = bt_task_manager_ros2.examples.ex_moveit_arm:main",
            "gripper = bt_task_manager_ros2.examples.ex_moveit_gripper:main",
            "grasp_node = bt_task_manager_ros2.scripts.grasp_node:main",
            "controller_node = bt_task_manager_ros2.scripts.controller_node:main",
            "aruco_detector_node = bt_task_manager_ros2.scripts.aruco_detector_node:main",
            "test = bt_task_manager_ros2.scripts.test:main",
        ],
    },
)
