import sys
import rclpy
import rclpy.logging
import py_trees
import py_trees_ros
from bt_task_manager_ros2.moveit_gripper import *

xarm_gripper_config = {
    "group_name": "xarm_gripper",
    "joint_names": ["drive_joint"],
    "open_joint_positions": [0.0],
    "close_joint_positions": [0.88],
    "action_name": "/xarm_gripper_traj_controller/follow_joint_trajectory",
}

gen3_robotiq_2f_85_config = {
    "group_name": "gripper",
    "joint_names": ["kortex_robotiq_85_left_knuckle_joint"],
    "open_joint_positions": [0.0],
    "close_joint_positions": [0.8],
    "action_name": "/robotiq_gripper_controller/gripper_cmd",
}

def create_queue_tree():
    main_sequence = py_trees.composites.Sequence(name="main_sequence", memory=True)
    main_sequence.add_children(
        [
            MoveitGripper("open gipper", gen3_robotiq_2f_85_config, "open"),
            MoveitGripper("close gripper", gen3_robotiq_2f_85_config, "close"),
        ]
    )
    return main_sequence


def main():
    rclpy.init()
    node = rclpy.create_node("example")
    logger = node.get_logger()
    logger.info("example node started")

    root = create_queue_tree()
    tree = py_trees_ros.trees.BehaviourTree(root=root, unicode_tree_debug=True)

    try:
        tree.setup(node=node, timeout=15)
    except py_trees_ros.exceptions.TimedOutError as e:
        logger.error("failed to setup the tree, aborting [{}]".format(str(e)))
        tree.shutdown()
        rclpy.try_shutdown()
        sys.exit(1)
    except KeyboardInterrupt:
        logger.error("tree setup interrupted")
        tree.shutdown()
        rclpy.try_shutdown()
        sys.exit(1)

    tree.tick_tock(period_ms=10.0)

    try:
        rclpy.spin(node)
    except (KeyboardInterrupt, rclpy.executors.ExternalShutdownException):
        pass
    finally:
        tree.shutdown()
        rclpy.try_shutdown()


if __name__ == "__main__":
    main()
