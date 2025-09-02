import py_trees
from geometry_msgs.msg import Twist
import time


def get_cmd_msg(direction, speed):
    """
    Generate a Twist message based on the given direction and speed.

    Parameters:
    - direction (str): The direction in which the robot should move ("forward", "backward", "rotate_left", "rotate_right").
    - speed (float): The speed at which the robot should move.

    Returns:
    - Twist: A Twist message containing the linear and angular velocities based on the input direction and speed.
    """
    cmd_vel_msg = Twist()
    if direction == "forward":
        cmd_vel_msg.linear.x = speed
    elif direction == "backward":
        cmd_vel_msg.linear.x = -speed
    elif direction == "rotate_left":
        cmd_vel_msg.angular.z = speed / 180 * 3.141592654
    elif direction == "rotate_right":
        cmd_vel_msg.angular.z = -speed / 180 * 3.141592654
    return cmd_vel_msg


class ChassisMove(py_trees.behaviour.Behaviour):
    def __init__(self, name, direction=None, distance=None, speed=0.1, cmd_topic="/cmd_vel"):
        super(ChassisMove, self).__init__(name=name)
        self.bb = self.attach_blackboard_client()
        self.cmd_topic = cmd_topic
        self.direction = direction
        self.distance = distance
        self.speed = speed
        if distance is None:
            self.static_goal = False
        else:
            self.static_goal = True

    def setup(self, **kwargs):
        self.node = kwargs["node"]
        self.cmd_vel_pub = self.node.create_publisher(Twist, self.cmd_topic, 10)
        self.feedback_message = "cmd_vel_pub created"

    def initialise(self):
        if self.static_goal:
            return
        try:
            self.direction = self.bb.get("direction")
            self.distance = self.bb.get("distance")
            self.speed = self.bb.get("speed")
        except Exception as e:
            self.logger.warning("[Ex] get param exception: {}".format(e))

    def update(self) -> py_trees.common.Status:
        if self.direction is None or self.distance is None or self.speed is None:
            return py_trees.common.Status.FAILURE
        if self.status != py_trees.common.Status.RUNNING:
            self.cmd_vel_msg = get_cmd_msg(self.direction, self.speed)
            self.cmd_vel_pub.publish(self.cmd_vel_msg)
            self.start_time = time.time()
            return py_trees.common.Status.RUNNING

        elapsed_time = time.time() - self.start_time
        self.distance_moved = self.speed * elapsed_time
        self.logger.info("Distance moved: {} ".format(self.distance_moved))
        if self.distance_moved >= self.distance:
            self.cmd_vel_msg = get_cmd_msg("stop", 0)
            self.cmd_vel_pub.publish(self.cmd_vel_msg)
            return py_trees.common.Status.SUCCESS

        self.cmd_vel_pub.publish(self.cmd_vel_msg)
        return py_trees.common.Status.RUNNING

    def terminate(self, new_status: py_trees.common.Status):
        if not self.static_goal:
            self.distance = None
            self.direction = None
            self.speed = None
