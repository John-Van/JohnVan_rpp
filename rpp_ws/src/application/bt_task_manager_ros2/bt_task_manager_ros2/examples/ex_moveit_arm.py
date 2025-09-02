import sys
import rclpy
import rclpy.logging
import py_trees
import py_trees_ros
from bt_task_manager_ros2.moveit_arm import *

xarm6_config = {
    "group_name": "xarm6",
    "base_link_name": "link_base",
    "joint_names": ["joint1", "joint2", "joint3", "joint4", "joint5", "joint6"],
    "end_effector_name": "link6",
    "planner_id": "RRTConnectkConfigDefault",
    "max_velocity": 0.5,
    "max_acceleration": 0.5,
    "named_poses": {
        "home": [0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
        "pose1": [-60.0, 100.0, -145.0, -145.0, 51.0, -36.0],
    },
}

gen3_config = {
    "group_name": "right_arm",
    "base_link_name": "base_link",
    "joint_names": ["right_joint_1", "right_joint_2", "right_joint_3", "right_joint_4", "right_joint_5", "right_joint_6", "right_joint_7"],
    "end_effector_name": "right_end_effector_link",
    "planner_id": "RRTConnectkConfigDefault",
    "max_velocity": 0.2,
    "max_acceleration": 0.2,
    "named_poses": {
        "zero": [0,0,0,0,0,0,0],
        "home": [-120,90,-90,120,0,40,0],
        "reset_pose": [-120,90,-90,120,0,40,-45],
        "detect_pose": [-120,90,-90,120,0,40,-90],
        "place_pose": [-120,90,-90,120,0,40,-180]
    },
}

def create_queue_tree():
    main_sequence = py_trees.composites.Sequence(
        name="main_sequence",
        memory=True,
        children=[
            # ArmMovetoPose("ArmMovetoPose1", gen3_config, [0.14, -0.334, 0.9, -90.0, 0.0, -90]),
            # ArmMovetoPose("ArmMovetoPose2", gen3_config, [0.2, 0.2, 0.2, 180.0, 0.0, 0.0]),
            # ArmMoveJoints("ArmMovetoPose2", gen3_config, [-60.0, 100.0, -145.0, -145.0, 51.0, -36.0]),
            # ArmMoveJoints("ArmMovetoPose2", gen3_config, [0.0, 0.0, 0.0, 0.0, 0.0, 92.0]),
            # ArmMovetoPoseInCartesian("ArmMovetoPose1", gen3_config, [0.4, -0.2, 0.2, 180.0, 0.0, 0.0]),
            # ArmMovetoPoseInCartesian("ArmMovetoPose2", gen3_config, [0.4, 0.2, 0.2, 180.0, 0.0, 0.0]),
            # ArmMoveEndInCartesianByTwist("ArmMovetoPose1",  [0.0, 0.0, 0.1, 0.0, 0.0, 0.0]),
            # ArmMoveEndInCartesianByTwist("ArmMovetoPose2",  [0.0, 0.0, -0.1, 0.0, 0.0, 0.0]),
            # ArmMoveEndInCartesian("ArmMovetoPose1", gen3_config, [0.0, 0.0, 0.1, 0.0, 0.0, 0.0]),
            # ArmMoveEndInCartesian("ArmMovetoPose1", gen3_config, [0.1, 0.0, 0.0, 0.0, 0.0, 0.0]),
            # ArmMoveEndInCartesian("ArmMovetoPose1", gen3_config, [-0.1, 0.0, 0.0, 0.0, 0.0, 0.0]),
            # ArmMoveEndInCartesian("ArmMovetoPose1", gen3_config, [0.0, 0.0, -0.1, 0.0, 0.0, 0.0]),
            # ArmMoveEndInCartesian("ArmMovetoPose2", gen3_config, [0.0, 0.0, -0.1, 0.0, 0.0, 0.0]),
            ArmMoveToName("ArmMoveToplace_pose", gen3_config, "zero"),
            # ArmMoveToName("ArmMoveTohome11", gen3_config, "home"),
            # ArmMoveToName("ArmMoveToreset", gen3_config, "reset_pose"),
            # ArmMoveToName("ArmMoveTodetect_pose", gen3_config, "detect_pose"),
            # ArmMoveToName("ArmMoveToplace_pose", gen3_config, "place_pose"),
        ],
    )
    return main_sequence

task_end = False

def post_tick_handler(tree):
    global task_end
    sys.stdout.write("\033[2J\033[H")
    sys.stdout.flush()
    print(tree.root.status)
    print(py_trees.display.ascii_tree(tree.root))
    if tree.root.status == py_trees.common.Status.SUCCESS:
        task_end = True
        print("success")
        tree.shutdown()


def main():
    global task_end
    rclpy.init()
    node = rclpy.create_node("example")
    logger = node.get_logger()
    logger.info("example node started")

    root = create_queue_tree()
    tree = py_trees_ros.trees.BehaviourTree(root=root, unicode_tree_debug=False)

    try:
        tree.setup(node=node, timeout=15)
        tree.tick_tock(period_ms=100.0,post_tick_handler=post_tick_handler)
    except py_trees_ros.exceptions.TimedOutError as e:
        logger.error("failed to setup the tree, aborting [{}]".format(str(e)))
        tree.shutdown()
        rclpy.shutdown()
        sys.exit(1)
    except KeyboardInterrupt:
        logger.error("tree setup interrupted")
        tree.shutdown()
        rclpy.shutdown()
        sys.exit(1)

    # try:
    #     rclpy.spin(node)
    # except (KeyboardInterrupt, rclpy.executors.ExternalShutdownException):
    #     pass
    # finally:
    #     tree.shutdown()
    #     rclpy.try_shutdown()
    while(rclpy.ok() and task_end == False):
        rclpy.spin_once(node)
    print("quit")

if __name__ == "__main__":
    main()
