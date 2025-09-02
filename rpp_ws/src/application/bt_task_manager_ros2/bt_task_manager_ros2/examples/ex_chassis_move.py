import py_trees
import py_trees_ros
import rclpy
import sys
from bt_task_manager_ros2.chassis_move import ChassisMove


def create_queue_tree():
    main_sequence = py_trees.composites.Sequence(name="main_sequence", memory=True)
    main_sequence.add_children(
        [
            ChassisMove("forward", "forward", 0.2, 0.1),
            ChassisMove("backward", "backward", 0.2, 0.1),
            ChassisMove("rotate_left", "rotate_left", 90, 10),
            ChassisMove("rotate_right", "rotate_right", 90, 10),
        ]
    )
    root = py_trees.decorators.OneShot("one_shot", main_sequence, py_trees.common.OneShotPolicy.ON_SUCCESSFUL_COMPLETION)
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
    rclpy.init()
    node = rclpy.create_node("example")
    logger = node.get_logger()
    logger.info("example node started")

    root = create_queue_tree()
    tree = py_trees_ros.trees.BehaviourTree(root=root, unicode_tree_debug=True)

    try:
        tree.setup(node=node, timeout=15)
        tree.tick_tock(period_ms=100.0,post_tick_handler=post_tick_handler)
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
