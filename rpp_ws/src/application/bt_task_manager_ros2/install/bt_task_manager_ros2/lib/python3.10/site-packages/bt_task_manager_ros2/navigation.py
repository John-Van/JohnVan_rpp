from rclpy import clock
from rclpy.action import ActionClient
import py_trees
import tf_transformations as tft
from geometry_msgs.msg import PoseStamped
from nav2_msgs.action import NavigateToPose
import threading
import time

from action_msgs.msg import GoalStatus


def create_navigation_goal_pose(x, y, theta):
    """create a navigation goal pose from a 2D navigation pose"""
    angle = (theta + 180.0) % 360 - 180
    goal = PoseStamped()
    goal.header.frame_id = "map"
    goal.header.stamp = clock.Clock().now().to_msg()
    goal.pose.position.x = x
    goal.pose.position.y = y
    quat = tft.quaternion_from_euler(0, 0, angle / 180 * 3.141592654)
    goal.pose.orientation.x = quat[0]
    goal.pose.orientation.y = quat[1]
    goal.pose.orientation.z = quat[2]
    goal.pose.orientation.w = quat[3]
    return goal


class NavigationToPose(py_trees.behaviour.Behaviour):
    def __init__(self, name, goal, action_namespace=""):
        super(NavigationToPose, self).__init__(name=name)
        self.nav_to_pose_client = None
        self.move_down = False
        self.move_success = False
        self.cancel_requested = False
        self.goal = goal
        self.action_namespace = action_namespace
        self.logger.info("task name is [{}], goal is [{}]".format(name, goal))

    def setup(self, **kwargs):
        self.node = kwargs["node"]
        self.nav_to_pose_client = ActionClient(self.node, NavigateToPose, "navigate_to_pose")

    def initialise(self):
        self.move_down = False
        self.move_success = False
        self.cancel_requested = False

    def update(self):
        if self.goal == None:
            self.logger.error("no goal [{}]".format(self.goal_name))
            return py_trees.common.Status.FAILURE
        if not self.nav_to_pose_client:
            self.feedback_message = "no action client, did you call setup() on your tree?"
            return py_trees.common.Status.INVALID
        if self.status != py_trees.common.Status.RUNNING:
            self.move_down = False
            self.move_success = False
            self.cancel_requested = False
            self.move_thread = threading.Thread(target=self.async_move)
            self.move_thread.start()
            return py_trees.common.Status.RUNNING
        if self.move_down == False:
            return py_trees.common.Status.RUNNING
        if self.move_success == True:
            return py_trees.common.Status.SUCCESS
        else:
            return py_trees.common.Status.FAILURE

    def async_move(self):

        (x, y, theta) = (self.goal[0], self.goal[1], self.goal[2])
        self.logger.info("Going to [{:.4f} {:.4f} {:.4f}]".format(x, y, theta))
        goal = NavigateToPose.Goal()
        goal.pose = create_navigation_goal_pose(x, y, theta)
        goal.behavior_tree = ""
        self.nav_to_pose_client.wait_for_server()

        # send goal req
        send_goal_future = self.nav_to_pose_client.send_goal_async(goal)
        # wait goal res
        while not send_goal_future.done():
            time.sleep(0.1)

        # check goal res
        self.goal_handle = send_goal_future.result()
        if self.goal_handle is None or not self.goal_handle.accepted:
            self.logger.error("go to pose was rejected!")
            self.move_success = False
            self.move_down = True
            return

        # wait action result
        result_future = self.goal_handle.get_result_async()
        while not result_future.done():
            if self.cancel_requested:
                self.logger.info("Canceling navigation goal...")
                cancel_future = self.goal_handle.cancel_goal_async()
                while not cancel_future.done():
                    time.sleep(0.1)
                cancel_response = cancel_future.result()
                if cancel_response.goals_canceling:
                    self.logger.info("Navigation goal successfully canceled.")
                else:
                    self.logger.error("Failed to cancel navigation goal.")
                self.move_success = False
                self.move_down = True
                return
            time.sleep(0.1)

        # check action result
        if result_future.result() is None:
            self.move_success = False
            self.move_down = True
            return

        status = result_future.result().status
        self.logger.info("navi status is {}".format(status))
        if status != GoalStatus.STATUS_SUCCEEDED:
            self.move_success = False
            self.move_down = True
            # TODO: add robot pose check, maybe goal is reached, but timeout
            self.logger.error("go to pose was failed!")
        else:
            self.move_success = True
            self.move_down = True
            self.logger.info("go to pose was successed!")

    def terminate(self, new_status):
        if self.status == py_trees.common.Status.RUNNING and new_status == py_trees.common.Status.INVALID:
            self.cancel_requested = True
        pass
