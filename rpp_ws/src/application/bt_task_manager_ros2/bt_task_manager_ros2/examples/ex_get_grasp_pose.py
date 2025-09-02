import sys
import rclpy
import rclpy.logging
import py_trees
import py_trees_ros
from bt_task_manager_ros2.get_grasp_pose import GetGraspPoseByTF


def create_queue_tree():
    main_sequence = py_trees.composites.Parallel(name="main_sequence", policy=py_trees.common.ParallelPolicy.SuccessOnAll())
    main_sequence.add_children(
        [
            GetGraspPoseByTF(
                "test", arm_base_frame="link_base", target_frame="goal", grasp_frame="grasp_pose", grasp_angles=[0, -90, 180], use_target_orient=False
            ),
            GetGraspPoseByTF(
                "test", arm_base_frame="link_base", target_frame="goal", grasp_frame="grasp_pose1", grasp_angles=[0, 0, 0], use_target_orient=True
            ),
        ]
    )
    root = py_trees.decorators.OneShot("one_shot", main_sequence, py_trees.common.OneShotPolicy.ON_SUCCESSFUL_COMPLETION)
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
