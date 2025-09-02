#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import sys
import os
import yaml
from ament_index_python.packages import get_package_share_directory

import rclpy
import rclpy.node
import py_trees
import py_trees_ros

import operator
import rclpy.qos
import std_msgs.msg as std_msgs
from bt_task_manager_ros2.moveit_gripper import *
from bt_task_manager_ros2.moveit_arm import *
from bt_task_manager_ros2.get_grasp_pose import *
from bt_task_manager_ros2.behaviours import *
from bt_task_manager_ros2.chassis_move import *
from bt_task_manager_ros2.navigation import *
from bt_task_manager_ros2.rb_decorators import *


class TaskManager(rclpy.node.Node):
    def __init__(self):
        super().__init__("controller_node")

        self.declare_parameter("task_cfg", "/home/rpp/rpp_ws/src/application/bt_task_manager_ros2/config/task_grasp.yaml")
        self.declare_parameter("moveit_arm_config", "gen3_config")
        self.declare_parameter("moveit_gripper_config", "gen3_robotiq_2f_85_config")
        self.declare_parameter("base_frame", "kortex_base_link")
        self.declare_parameter("target_frame", "aruco_marker_frame")
        self.declare_parameter("chassis_feed_depth", 0.2)
        self.declare_parameter("tcp_offset_x", 0.01)
        self.declare_parameter("tcp_offset_y", 0.0)
        self.declare_parameter("tcp_offset_z", -0.11)
        self.declare_parameter("arm_feed_depth", 0.1)
        self.declare_parameter("grasp_angles_x", 0.0)
        self.declare_parameter("grasp_angles_y", 0.0)
        self.declare_parameter("grasp_angles_z", 0.0)
        self.declare_parameter("use_target_orient", False)
        self.declare_parameter("grasp_angles", [90.0, 0.0, 90.0])

        self.task_cfg = self.get_parameter("task_cfg").value
        self.moveit_arm_config = self.get_parameter("moveit_arm_config").value
        self.moveit_gripper_config = self.get_parameter("moveit_gripper_config").value
        self.base_frame = self.get_parameter("base_frame").value
        self.target_frame = self.get_parameter("target_frame").value
        self.chassis_feed_depth = self.get_parameter("chassis_feed_depth").value
        self.tcp_offset_x = self.get_parameter("tcp_offset_x").value
        self.tcp_offset_y = self.get_parameter("tcp_offset_y").value
        self.tcp_offset_z = self.get_parameter("tcp_offset_z").value
        self.arm_feed_depth = self.get_parameter("arm_feed_depth").value
        self.grasp_angles_x = self.get_parameter("grasp_angles_x").value
        self.grasp_angles_y = self.get_parameter("grasp_angles_y").value
        self.grasp_angles_z = self.get_parameter("grasp_angles_z").value
        self.use_target_orient = self.get_parameter("use_target_orient").value
        self.grasp_angles = self.get_parameter("grasp_angles").value

        self.task_name = ""
        self.task_list = []
        self.task_repeat = False

        moveit_arm_configs = yaml.safe_load(
            open(os.path.join(get_package_share_directory("bt_task_manager_ros2"), "config", "moveit_arm_configs.yaml"))
        )
        moveit_gripper_configs = yaml.safe_load(
            open(os.path.join(get_package_share_directory("bt_task_manager_ros2"), "config", "moveit_gripper_configs.yaml"))
        )

        self.moveit_arm_config_dict = moveit_arm_configs[self.moveit_arm_config]
        self.moveit_gripper_config_dict = moveit_gripper_configs[self.moveit_gripper_config]

        self.running = False
        self.loop = False
        self.cmd = None
        self.current_job = None

        self.bb = py_trees.blackboard.Blackboard()
        self.loadTask(self.task_cfg)

        py_trees.logging.level = py_trees.logging.Level.INFO
        self.work_tree = self.create_work_tree(self.task_list)

        self.tree = py_trees_ros.trees.BehaviourTree(self.create_root_tree(self.work_tree))
        self.snapshot_visitor = py_trees.visitors.SnapshotVisitor()
        self.tree.visitors.append(self.snapshot_visitor)
        self.tree.add_pre_tick_handler(self.pre_tick_handler)
        self.tree.add_post_tick_handler(self.post_tick_handler)

        self.status_pub = self.create_publisher(std_msgs.String, "~/status", 5)
        self.result_pub = self.create_publisher(std_msgs.String, "~/result", 5)
        self.status = std_msgs.Int16()

    def setup(self, timeout):
        try:
            self.tree.setup(node=self, timeout=timeout)
        except py_trees_ros.exceptions.TimedOutError as e:
            self.get_logger().error("failed to setup the tree, aborting [{}]".format(str(e)))
            self.tree.shutdown()
            rclpy.try_shutdown()
            sys.exit(1)
        except KeyboardInterrupt:
            self.get_logger().error("tree setup interrupted")
            self.tree.shutdown()
            rclpy.try_shutdown()
            sys.exit(1)

    def pre_tick_handler(self, tree: py_trees.trees.BehaviourTree):
        pass

    def post_tick_handler(self, tree):
        work_tree = tree.root.children[1].children[0].children[1]
        work_state = work_tree.status
        self.get_logger().info("\033[2J\033[H")
        self.get_logger().info("working state : [{}]".format(work_state))
        msg = std_msgs.String(data=str(work_state)[7:])
        if work_state == py_trees.common.Status.SUCCESS or work_state == py_trees.common.Status.FAILURE:
            self.result_pub.publish(msg)
        self.status_pub.publish(msg)
        # pass
        self.get_logger().info(
            py_trees.display.ascii_tree(tree.root, visited=self.snapshot_visitor.visited, previously_visited=self.snapshot_visitor.visited)
        )

    def tick_tock(self):
        self.tree.tick_tock(100)

    def loadTask(self, cfg):
        rf = open(cfg, "r")
        crf = rf.read()
        rf.close()
        yaml_data = yaml.safe_load(crf)
        self.name = yaml_data["name"]
        self.repeat = yaml_data["repeat"]
        self.get_logger().info("task name is [{}], repeat is [{}]".format(self.name, self.repeat))
        self.bb.set("repeat", self.repeat)
        self.task_list = yaml_data["tasks"]
        self.get_logger().info("task_list : {}".format(self.task_list))

    def create_root_tree(self, work_subtree):
        topics2bb = py_trees.composites.Sequence("Topics2BB", True)
        start2bb = py_trees_ros.subscribers.EventToBlackboard(
            name="Start2BB",
            topic_name="~/dashboard/start",
            qos_profile=py_trees_ros.utilities.qos_profile_unlatched(),
            variable_name="event_start_button",
        )
        cancel2bb = py_trees_ros.subscribers.EventToBlackboard(
            name="Cancel2BB",
            topic_name="~/dashboard/cancel",
            qos_profile=py_trees_ros.utilities.qos_profile_unlatched(),
            variable_name="event_cancel_button",
        )
        tasks = py_trees.composites.Selector("Tasks", False)
        idle = py_trees.behaviours.Running(name="Idle")

        work = py_trees.composites.Sequence("Work", True)
        is_start_requested = py_trees.behaviours.CheckBlackboardVariableValue(
            name="Start?", check=py_trees.common.ComparisonExpression(variable="event_start_button", value=True, operator=operator.eq)
        )
        is_cancel_requested = py_trees.behaviours.CheckBlackboardVariableValue(
            name="Cancel?", check=py_trees.common.ComparisonExpression(variable="event_cancel_button", value=True, operator=operator.eq)
        )
        reset = self.create_reset_tree()

        work_loop = LoopWithCondition("Work Loop", work_subtree, "repeat")

        work_or_be_cancelled = py_trees.composites.Selector("Work or Be Cancelled", False)
        root = py_trees.composites.Parallel("Root", py_trees.common.ParallelPolicy.SuccessOnAll(synchronise=False))

        root.add_children([topics2bb, tasks])
        topics2bb.add_children([start2bb, cancel2bb])
        tasks.add_children([work, idle])
        work.add_children([is_start_requested, work_or_be_cancelled, reset])
        work_or_be_cancelled.add_children([is_cancel_requested, work_loop])

        return root

    def create_pick_tree(self, name):
        seq = py_trees.composites.Sequence(name, True)
        seq.add_children(
            [
                # MoveitGripper("open gripper", self.moveit_gripper_config_dict, "open"),
                ArmMoveToName("move to detect pose", self.moveit_arm_config_dict, goal="detect_pose"),
                GetGraspPoseByTF(
                    "get grasp pose",
                    arm_base_frame=self.base_frame,
                    target_frame=self.target_frame,
                    grasp_frame="grasp_pose",
                    feed_depth=self.arm_feed_depth,
                    offset_x=self.tcp_offset_x,
                    offset_y=self.tcp_offset_y,
                    offset_z=self.tcp_offset_z,
                    use_target_orient=self.use_target_orient,
                    grasp_angles=self.grasp_angles,
                ),
                ArmMovetoPose("move to grasp pose", self.moveit_arm_config_dict, goal_name="grasp_pose"),
                ArmMoveEndInCartesian("feed in", self.moveit_arm_config_dict, goal=[0, 0, self.arm_feed_depth, 0, 0, 0]),
                # MoveitGripper("close gripper", self.moveit_gripper_config_dict, "close"),
                ArmMoveEndInCartesian("feed out", self.moveit_arm_config_dict, goal=[0, 0, -self.arm_feed_depth, 0, 0, 0]),
                ArmMoveToName("move to detect pose", self.moveit_arm_config_dict, goal="detect_pose"),
            ]
        )
        return seq

    def create_place_tree(self, name):
        seq = py_trees.composites.Sequence(name, True)
        seq.add_children(
            [
                ArmMoveToName("move to place pose", self.moveit_arm_config_dict, goal="place_pose"),
                # MoveitGripper("open gripper", self.moveit_gripper_config_dict, "open"),
                # ArmMoveToName("move to detect pose", self.moveit_arm_config_dict, goal="detect_pose"),
            ]
        )
        return seq

    def create_work_tree(self, task_list):
        task_seq = py_trees.composites.Sequence("work_seq", True)
        index = 0
        for it in task_list:
            if "position" in it:
                navi = NavigationToPose(name="navigation_{}".format(index), goal=(it["position"]))
                task_seq.add_child(navi)
            if "action" in it:
                if it["action"] == "pick":
                    chassis_feed_in = ChassisMove("chassis_feed_in_{}".format(index), "forward", self.chassis_feed_depth, 0.1)
                    task_seq.add_child(chassis_feed_in)
                    pick = self.create_pick_tree("pick_{}".format(index))
                    task_seq.add_child(pick)
                    chassis_retreat_out = ChassisMove("chassis_retreat_out_{}".format(index), "backward", self.chassis_feed_depth, 0.1)
                    task_seq.add_child(chassis_retreat_out)
                if it["action"] == "place":
                    chassis_feed_in = ChassisMove("chassis_feed_in_{}".format(index), "forward", self.chassis_feed_depth, 0.1)
                    task_seq.add_child(chassis_feed_in)
                    place = self.create_place_tree("place_{}".format(index))
                    task_seq.add_child(place)
                    chassis_retreat_out = ChassisMove("chassis_retreat_out_{}".format(index), "backward", self.chassis_feed_depth, 0.1)
                    task_seq.add_child(chassis_retreat_out)
            index += 1
        return task_seq

    def create_reset_tree(self):
        return ArmMoveToName("move to reset", self.moveit_arm_config_dict, goal="reset_pose")


def main():
    # Start ROS node
    rclpy.init()

    controller = TaskManager()
    controller.setup(15)
    controller.get_logger().info("controller node start.")

    controller.tick_tock()

    try:
        rclpy.spin(controller)
    finally:
        rclpy.try_shutdown()


if __name__ == "__main__":
    main()
