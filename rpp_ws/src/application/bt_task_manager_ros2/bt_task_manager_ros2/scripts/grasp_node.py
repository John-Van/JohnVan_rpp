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

import std_msgs.msg as std_msgs
from bt_task_manager_ros2.moveit_gripper import *
from bt_task_manager_ros2.moveit_arm import *
from bt_task_manager_ros2.get_grasp_pose import *
from bt_task_manager_ros2.behaviours import *


"""
use dynamic subtrees to control workflow
tree struct:
                |root seq|
                /         \
        |work subTree |  |  idle  |

tick loop:
    |pre tick| : handle cmd except "stop", insert subTree to root seq
    |tick|
    |post tick| : handle "stop" cmd, pune subTree from root seq 

referenced from py_trees_ros/tutorials/eight.py

cmd : "pick", "place", "loop", "stop", "reset", "gripper_open", "gripper_close":
return : 1-success; 0 - activing; -1 - aborted(执行失败)
"""


class GraspNode(rclpy.node.Node):
    def __init__(self):
        super().__init__("grasp_node")
        self.declare_parameter("moveit_arm_config", "gen3_config")
        self.declare_parameter("moveit_gripper_config", "gen3_robotiq_2f_85_config")
        self.declare_parameter("base_frame", "kortex_base_link")
        self.declare_parameter("target_frame", "aruco_marker_frame")
        self.declare_parameter("tcp_offset_x", 0.01)
        self.declare_parameter("tcp_offset_y", 0.0)
        self.declare_parameter("tcp_offset_z", -0.11)
        self.declare_parameter("arm_feed_depth", 0.1)
        self.declare_parameter("grasp_angles_x", 0.0)
        self.declare_parameter("grasp_angles_y", 0.0)
        self.declare_parameter("grasp_angles_z", 0.0)
        self.declare_parameter("use_target_orient", False)
        self.declare_parameter("grasp_angles", [90.0, 0.0, 90.0])

        self.moveit_arm_config = self.get_parameter("moveit_arm_config").value
        self.moveit_gripper_config = self.get_parameter("moveit_gripper_config").value
        self.base_frame = self.get_parameter("base_frame").value
        self.target_frame = self.get_parameter("target_frame").value
        self.tcp_offset_x = self.get_parameter("tcp_offset_x").value
        self.tcp_offset_y = self.get_parameter("tcp_offset_y").value
        self.tcp_offset_z = self.get_parameter("tcp_offset_z").value
        self.arm_feed_depth = self.get_parameter("arm_feed_depth").value
        self.grasp_angles_x = self.get_parameter("grasp_angles_x").value
        self.grasp_angles_y = self.get_parameter("grasp_angles_y").value
        self.grasp_angles_z = self.get_parameter("grasp_angles_z").value
        self.use_target_orient = self.get_parameter("use_target_orient").value
        self.grasp_angles = self.get_parameter("grasp_angles").value

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

        self.cmd_sub = self.create_subscription(std_msgs.String, "~/cmd", self.recv_cmd_callback, 5)
        self.status_pub = self.create_publisher(std_msgs.Int16, "~/status", 5)
        self.status = std_msgs.Int16()

        py_trees.logging.level = py_trees.logging.Level.INFO
        self.tree = py_trees_ros.trees.BehaviourTree(self.create_root_tree())
        self.snapshot_visitor = py_trees.visitors.SnapshotVisitor()
        self.tree.visitors.append(self.snapshot_visitor)
        self.tree.add_pre_tick_handler(self.pre_tick_handler)
        self.tree.add_post_tick_handler(self.post_tick_handler)

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

    def create_pick_tree(self):
        seq = py_trees.composites.Sequence(
            "Pick",
            True,
            children=[
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
                # ArmMoveToName("move to detect pose", self.moveit_arm_config_dict, goal="detect_pose"),
            ],
        )
        return seq

    def create_place_tree(self):
        seq = py_trees.composites.Sequence(
            "Place",
            True,
            [
                ArmMoveToName("move to place pose", self.moveit_arm_config_dict, goal="place_pose"),
                # MoveitGripper("open gripper", self.moveit_gripper_config_dict, "open"),
                # ArmMoveToName("move to detect pose", self.moveit_arm_config_dict, goal="detect_pose"),
            ],
        )
        return seq

    def create_pick_and_place_tree(self):
        seq = py_trees.composites.Sequence("Pick and Place", memory=True, children=[self.create_pick_tree(), self.create_place_tree()])
        return seq

    def create_reset_tree(self):
        return ArmMoveToName("move to reset", self.moveit_arm_config_dict, goal="reset_pose")

    def create_work_tree(self, cmd):
        if cmd == "pick":
            return py_trees.composites.Sequence("Pick and Reset", memory=True, children=[self.create_pick_tree()])
        elif cmd == "place":
            return py_trees.composites.Sequence("Pick and Reset", memory=True, children=[self.create_place_tree()])
        elif cmd == "loop":
            return py_trees.composites.Sequence("Loop and Reset", memory=True, children=[self.create_pick_and_place_tree()])
        elif cmd == "reset":
            return ArmMoveToName("move to reset", self.moveit_arm_config_dict, goal="reset_pose")
        # elif cmd == "gripper_open":
            # return MoveitGripper("open gripper", self.moveit_gripper_config_dict, "open")
        # elif cmd == "gripper_close":
            # return MoveitGripper("close gripper", self.moveit_gripper_config_dict, "close")
        else:
            return None

    def create_root_tree(self):
        root = py_trees.composites.Selector("Root", False)
        idle = py_trees.behaviours.Running(name="Idle")
        root.add_children(children=[idle])
        return root

    def pre_tick_handler(self, tree: py_trees.trees.BehaviourTree):
        # self.get_logger().info("running!")
        if not self.cmd or self.cmd == "stop":
            return
        # deel start cmd
        if not self.current_job:
            self.current_job = self.create_work_tree(self.cmd)
            if not self.current_job:
                self.get_logger().error("error cmd {}, can not create job".format(self.cmd))
                self.cmd = None
                self.running = False
                self.loop = False
                self.current_job = None
                return
            try:
                py_trees.trees.setup(root=self.current_job, timeout=py_trees.common.Duration.INFINITE.value, node=self)
            except Exception as e:
                self.get_logger().error("failed to setup the tree, aborting [{}]".format(str(e)))
                self.cmd = None
                self.running = False
                self.loop = False
                self.current_job = None
                return
            tree.insert_subtree(self.current_job, tree.root.id, 0)
            self.cmd = None
            return

    def post_tick_handler(self, tree: py_trees.trees.BehaviourTree):
        self.get_logger().info("\033[2J\033[H")
        self.get_logger().info(
            py_trees.display.ascii_tree(tree.root, visited=self.snapshot_visitor.visited, previously_visited=self.snapshot_visitor.visited)
        )
        if not self.running:
            return
        job = tree.root.children[-2]
        if job.status == py_trees.common.Status.RUNNING:
            # deel stop cmd in running
            if self.cmd == "stop":
                self.get_logger().info("Stop {}!".format(job.name))
                tree.prune_subtree(job.id)
                self.cmd = None
                self.running = False
                self.loop = False
                self.current_job = None
                self.status.data = 1
                self.status_pub.publish(self.status)
                return
            self.status.data = 0
            self.status_pub.publish(self.status)
        elif job.status == py_trees.common.Status.SUCCESS:
            self.get_logger().info("{} Success!".format(job.name))
            if self.loop == True:
                return
            tree.prune_subtree(job.id)
            self.cmd = None
            self.running = False
            self.loop = False
            self.current_job = None
            self.status.data = 1
            self.status_pub.publish(self.status)
        elif tree.root.status == py_trees.common.Status.FAILURE:
            self.get_logger().info("{} Failed!".format(job.name))
            tree.prune_subtree(job.id)
            self.cmd = None
            self.running = False
            self.loop = False
            self.current_job = None
            self.status.data = -1
            self.status_pub.publish(self.status)
        # change to reset mode

    def recv_cmd_callback(self, msg):
        self.get_logger().info("recv cmd: {}".format(msg.data))
        if msg.data == "":
            return
        if self.running:
            if msg.data == "stop":
                self.cmd = msg.data
            return
        self.cmd = msg.data
        if self.cmd == "loop":
            self.loop = True
        self.running = True

    def tick_tock(self):
        self.tree.tick_tock(100)


def main():
    # Start ROS node
    rclpy.init()

    grasp_server = GraspNode()
    grasp_server.setup(15)
    grasp_server.get_logger().info("grasp node start.")

    grasp_server.tick_tock()

    try:
        rclpy.spin(grasp_server)
    finally:
        rclpy.try_shutdown()


if __name__ == "__main__":
    main()
