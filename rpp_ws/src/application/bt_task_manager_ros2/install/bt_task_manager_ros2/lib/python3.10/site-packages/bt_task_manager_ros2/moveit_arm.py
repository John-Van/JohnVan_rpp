import math
import time
import py_trees

import threading

from pymoveit2 import MoveIt2
from geometry_msgs.msg import Pose
import tf_transformations as tft
from geometry_msgs.msg import Twist
from controller_manager_msgs.srv import SwitchController


class ArmMovetoPose(py_trees.behaviour.Behaviour):
    def __init__(self, name, moveit_config: dict, goal=None, goal_name=None):
        super(ArmMovetoPose, self).__init__(name)
        self.bb = self.attach_blackboard_client()
        self.moveit_config = moveit_config
        self.group_name = self.moveit_config["group_name"]
        self.base_link_name = self.moveit_config["base_link_name"]
        self.joint_names = self.moveit_config["joint_names"]
        self.end_effector_name = self.moveit_config["end_effector_name"]
        self.planner_id = self.moveit_config["planner_id"]
        self.max_velocity = self.moveit_config["max_velocity"]
        self.max_acceleration = self.moveit_config["max_acceleration"]
        self.goal_name = goal_name
        self.goal = goal
        if goal is None:
            self.static_goal = False
        else:
            self.static_goal = True
        self.move_down = False
        self.move_success = False

    def setup(self, **kwargs):
        self.node = kwargs["node"]
        # init pymoveit2, please read pymoveit2 docs and codes
        self.moveit2 = MoveIt2(
            node=self.node,
            joint_names=self.joint_names,
            base_link_name=self.base_link_name,
            end_effector_name=self.end_effector_name,
            group_name=self.group_name,
        )
        self.moveit2.planner_id = self.planner_id
        self.moveit2.max_velocity = self.max_velocity
        self.moveit2.max_acceleration = self.max_acceleration
        # param in cartesian planning request
        self.moveit2.cartesian_avoid_collisions = False
        self.moveit2.cartesian_jump_threshold = 0.0


    def initialise(self):
        if self.static_goal:
            return
        if not self.goal_name:
            return
        try:
            self.bb.register_key(key=self.goal_name, access=py_trees.common.Access.WRITE)
            self.goal = self.bb.get(self.goal_name)
            self.bb.set(self.goal_name, None,overwrite=True)
        except Exception as e:
            self.logger.error("[Ex] get xarm_target_pose exception: {}".format(e))

    def update(self) -> py_trees.common.Status:
        if self.goal == None:
            self.logger.error("no goal [{}]".format(self.goal_name))
            return py_trees.common.Status.FAILURE
        if self.status != py_trees.common.Status.RUNNING:
            self.move_down = False
            self.move_success = False
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
        goal = Pose()
        goal.position.x = self.goal[0]
        goal.position.y = self.goal[1]
        goal.position.z = self.goal[2]
        quat = tft.quaternion_from_euler(self.goal[3] / 180 * 3.141592654, self.goal[4] / 180 * 3.141592654, self.goal[5] / 180 * 3.141592654)
        goal.orientation.x = quat[0]
        goal.orientation.y = quat[1]
        goal.orientation.z = quat[2]
        goal.orientation.w = quat[3]

        self.moveit2.move_to_pose(goal)

        ret = self.moveit2.wait_until_executed()
        if ret:
            self.move_success = True
        else:
            self.move_success = False
        self.move_down = True

    def terminate(self, new_status):
        if not self.static_goal:
            self.goal = None


class ArmMovetoPoseInCartesian(py_trees.behaviour.Behaviour):
    def __init__(self, name, moveit_config: dict, goal=None, goal_name=None):
        super(ArmMovetoPoseInCartesian, self).__init__(name)
        self.bb = self.attach_blackboard_client()
        self.moveit_config = moveit_config
        self.group_name = self.moveit_config["group_name"]
        self.base_link_name = self.moveit_config["base_link_name"]
        self.joint_names = self.moveit_config["joint_names"]
        self.end_effector_name = self.moveit_config["end_effector_name"]
        self.planner_id = self.moveit_config["planner_id"]
        self.max_velocity = self.moveit_config["max_velocity"]
        self.max_acceleration = self.moveit_config["max_acceleration"]
        self.goal_name = goal_name
        self.goal = goal
        if goal is None:
            self.static_goal = False
        else:
            self.static_goal = True
        self.move_down = False
        self.move_success = False

    def setup(self, **kwargs):
        self.node = kwargs["node"]
        # init pymoveit2, please read pymoveit2 docs and codes
        self.moveit2 = MoveIt2(
            node=self.node,
            joint_names=self.joint_names,
            base_link_name=self.base_link_name,
            end_effector_name=self.end_effector_name,
            group_name=self.group_name,
        )
        self.moveit2.planner_id = self.planner_id
        self.moveit2.max_velocity = self.max_velocity
        self.moveit2.max_acceleration = self.max_acceleration
        # param in cartesian planning request
        self.moveit2.cartesian_avoid_collisions = False
        self.moveit2.cartesian_jump_threshold = 0.0

    def initialise(self):
        if self.static_goal:
            return
        if not self.goal_name:
            return
        try:
            self.goal = self.bb.get(self.goal_name)
            self.bb.set(self.goal_name, None)
        except Exception as e:
            self.logger.error("[Ex] get xarm_target_pose exception: {}".format(e))

    def update(self) -> py_trees.common.Status:
        if self.goal == None:
            self.logger.error("no goal [{}]".format(self.goal_name))
            return py_trees.common.Status.FAILURE
        if self.status != py_trees.common.Status.RUNNING:
            self.move_down = False
            self.move_success = False
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
        goal = Pose()
        goal.position.x = self.goal[0]
        goal.position.y = self.goal[1]
        goal.position.z = self.goal[2]
        quat = tft.quaternion_from_euler(self.goal[3] / 180 * 3.141592654, self.goal[4] / 180 * 3.141592654, self.goal[5] / 180 * 3.141592654)
        goal.orientation.x = quat[0]
        goal.orientation.y = quat[1]
        goal.orientation.z = quat[2]
        goal.orientation.w = quat[3]

        self.moveit2.move_to_pose(goal, cartesian=True)

        ret = self.moveit2.wait_until_executed()
        if ret:
            self.move_success = True
        else:
            self.move_success = False
        self.move_down = True

    def terminate(self, new_status):
        if not self.static_goal:
            self.goal = None


class ArmMoveEndInCartesian(py_trees.behaviour.Behaviour):
    def __init__(self, name, moveit_config: dict, goal=None, goal_name=None):
        super(ArmMoveEndInCartesian, self).__init__(name)
        self.bb = self.attach_blackboard_client()
        self.moveit_config = moveit_config
        self.group_name = self.moveit_config["group_name"]
        self.base_link_name = self.moveit_config["base_link_name"]
        self.joint_names = self.moveit_config["joint_names"]
        self.end_effector_name = self.moveit_config["end_effector_name"]
        self.planner_id = self.moveit_config["planner_id"]
        self.max_velocity = self.moveit_config["max_velocity"]
        self.max_acceleration = self.moveit_config["max_acceleration"]
        self.goal_name = goal_name
        self.goal = goal
        if goal is None:
            self.static_goal = False
        else:
            self.static_goal = True
        self.move_down = False
        self.move_success = False

    def setup(self, **kwargs):
        self.node = kwargs["node"]
        # init pymoveit2, please read pymoveit2 docs and codes
        self.moveit2 = MoveIt2(
            node=self.node,
            joint_names=self.joint_names,
            base_link_name=self.base_link_name,
            end_effector_name=self.end_effector_name,
            group_name=self.group_name,
        )
        self.moveit2.planner_id = self.planner_id
        self.moveit2.max_velocity = self.max_velocity
        self.moveit2.max_acceleration = self.max_acceleration
        # param in cartesian planning request
        self.moveit2.cartesian_avoid_collisions = False
        self.moveit2.cartesian_jump_threshold = 0.0


    def initialise(self):
        if self.static_goal:
            return
        if not self.goal_name:
            return
        try:
            self.goal = self.bb.get(self.goal_name)
            self.bb.set(self.goal_name, None)
        except Exception as e:
            self.logger.error("[Ex] get xarm_target_pose exception: {}".format(e))

    def update(self) -> py_trees.common.Status:
        if self.goal == None:
            self.logger.error("no goal [{}]".format(self.goal_name))
            return py_trees.common.Status.FAILURE
        if self.status != py_trees.common.Status.RUNNING:
            self.move_down = False
            self.move_success = False
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
        goal = Pose()
        goal.position.x = self.goal[0]*1.0
        goal.position.y = self.goal[1]*1.0
        goal.position.z = self.goal[2]*1.0
        quat = tft.quaternion_from_euler(self.goal[3] / 180 * 3.141592654, self.goal[4] / 180 * 3.141592654, self.goal[5] / 180 * 3.141592654)
        goal.orientation.x = quat[0]*1.0
        goal.orientation.y = quat[1]*1.0
        goal.orientation.z = quat[2]*1.0
        goal.orientation.w = quat[3]*1.0

        self.moveit2.move_to_pose(goal, frame_id=self.end_effector_name, cartesian=True)

        ret = self.moveit2.wait_until_executed()
        if ret:
            self.move_success = True
        else:
            self.move_success = False
        self.move_down = True

    def terminate(self, new_status):
        if not self.static_goal:
            self.goal = None

# only surpport one dimension motion
class ArmMoveEndInCartesianByTwist(py_trees.behaviour.Behaviour):
    def __init__(self, name, goal=None, goal_name=None, twist_controller="twist_controller", trajectory_controller="joint_trajectory_controller", twist_topic="/twist_controller/commands", switch_controller_service="/controller_manager/switch_controller"):
        super(ArmMoveEndInCartesianByTwist, self).__init__(name)
        self.bb = self.attach_blackboard_client()
        self.goal_name = goal_name
        self.goal = goal
        self.static_goal = goal is not None
        self.twist_controller = twist_controller
        self.trajectory_controller = trajectory_controller
        self.twist_topic = twist_topic
        self.switch_controller_service = switch_controller_service
        self.move_down = False
        self.move_success = False

    def setup(self, **kwargs):
        self.node = kwargs["node"]
        self.publisher = self.node.create_publisher(Twist, self.twist_topic, 10)
        self.switch_controller_client = self.node.create_client(SwitchController, self.switch_controller_service)
        if not self.switch_controller_client.wait_for_service(timeout_sec=10.0):
            self.logger.error("Service /controller_manager/switch_controller not available!")


    def initialise(self):
        if self.static_goal:
            return
        if not self.goal_name:
            return
        try:
            self.goal = self.bb.get(self.goal_name)
            self.bb.set(self.goal_name, None)
        except Exception as e:
            self.logger.error("[Ex] get xarm_target_pose exception: {}".format(e))

    def update(self) -> py_trees.common.Status:
        if self.goal is None:
            self.logger.error("no goal [{}]".format(self.goal_name))
            return py_trees.common.Status.FAILURE

        if self.status != py_trees.common.Status.RUNNING:
            self.move_down = False
            self.move_success = False
            self.move_thread = threading.Thread(target=self.async_move)
            self.move_thread.start()
            return py_trees.common.Status.RUNNING

        if not self.move_down:
            return py_trees.common.Status.RUNNING

        if self.move_success:
            return py_trees.common.Status.SUCCESS
        else:
            return py_trees.common.Status.FAILURE

    def async_move(self):
        try:
            if not self.switch_controllers([self.twist_controller], [self.trajectory_controller]):
                self.logger.error("Failed to switch to velocity controller!")
                self.move_success = False
                self.move_down = True
                return

            velocity = 0.02
            duration = 0  
            twist = Twist()
            if math.fabs(self.goal[0]) >1e-10:
                twist.linear.x = velocity if self.goal[0]>0 else -velocity
                duration = math.fabs(self.goal[0]) / velocity
            elif math.fabs(self.goal[1]) >1e-10:
                twist.linear.y = velocity if self.goal[1]>0 else -velocity
                duration = math.fabs(self.goal[1]) / velocity
            elif math.fabs(self.goal[2]) >1e-10:
                twist.linear.z = velocity if self.goal[2]>0 else -velocity
                duration = math.fabs(self.goal[2]) / velocity
            elif math.fabs(self.goal[3]) >1e-10:
                twist.angular.x = velocity if self.goal[3]>0 else -velocity
                duration = math.fabs(self.goal[3]) / velocity
            elif math.fabs(self.goal[4]) >1e-10:
                twist.angular.y = velocity if self.goal[4]>0 else -velocity
                duration = math.fabs(self.goal[4]) / velocity
            elif math.fabs(self.goal[5]) >1e-10:
                twist.angular.z = velocity if self.goal[5]>0 else -velocity
                duration = math.fabs(self.goal[5]) / velocity

            # 发布速度命令
            start_time = time.time()
            while time.time() - start_time < duration:
                self.publisher.publish(twist)
                time.sleep(0.1)  # 发布频率 10 Hz
            self.stop_motion()
            if not self.switch_controllers([self.trajectory_controller], [self.twist_controller]):
                self.logger.error("Failed to switch back to trajectory controller!")
                self.move_success = False
                self.move_down = True
                return

            self.move_success = True
            self.move_down = True
        except Exception as e:
            self.logger.error(f"Movement error: {e}")
            self.move_success = False
            self.move_down = True

    def stop_motion(self):
        stop_twist = Twist()
        self.publisher.publish(stop_twist)
        self.logger.info("Motion stopped.")

    def switch_controllers(self, activate, deactivate):
        request = SwitchController.Request()
        request.activate_controllers = activate
        request.deactivate_controllers = deactivate
        request.strictness = 1  # STRICT 模式

        response = self.switch_controller_client.call(request)
        if response.ok:
            self.logger.info(f"Switched controllers: activate={activate}, deactivate={deactivate}")
            return True
        else:
            self.logger.error(f"Failed to switch controllers: activate={activate}, deactivate={deactivate}")
            return False

    def terminate(self, new_status):
        if not self.static_goal:
            self.goal = None

class ArmMoveJoints(py_trees.behaviour.Behaviour):
    def __init__(self, name, moveit_config: dict, goal=None, goal_name=None):
        super(ArmMoveJoints, self).__init__(name)
        self.bb = self.attach_blackboard_client()
        self.moveit_config = moveit_config
        self.group_name = self.moveit_config["group_name"]
        self.base_link_name = self.moveit_config["base_link_name"]
        self.joint_names = self.moveit_config["joint_names"]
        self.end_effector_name = self.moveit_config["end_effector_name"]
        self.planner_id = self.moveit_config["planner_id"]
        self.max_velocity = self.moveit_config["max_velocity"]
        self.max_acceleration = self.moveit_config["max_acceleration"]
        self.goal_name = goal_name
        self.goal = goal
        if goal is None:
            self.static_goal = False
        else:
            self.static_goal = True
        self.move_down = False
        self.move_success = False

    def setup(self, **kwargs):
        self.node = kwargs["node"]
        # init pymoveit2, please read pymoveit2 docs and codes
        self.moveit2 = MoveIt2(
            node=self.node,
            joint_names=self.joint_names,
            base_link_name=self.base_link_name,
            end_effector_name=self.end_effector_name,
            group_name=self.group_name,
        )
        self.moveit2.planner_id = self.planner_id
        self.moveit2.max_velocity = self.max_velocity
        self.moveit2.max_acceleration = self.max_acceleration

    def initialise(self):
        if self.static_goal:
            return
        if not self.goal_name:
            return
        try:
            self.goal = self.bb.get(self.goal_name)
            self.bb.set(self.goal_name, None)
        except Exception as e:
            self.logger.error("[Ex] get xarm_target_pose exception: {}".format(e))

    def update(self) -> py_trees.common.Status:
        if self.goal == None:
            self.logger.error("no goal [{}]".format(self.goal_name))
            return py_trees.common.Status.FAILURE
        if self.status != py_trees.common.Status.RUNNING:
            self.move_down = False
            self.move_success = False
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
        # wait for joints state
        while self.moveit2.joint_state is None:
            time.sleep(0.1)

        goal = self.goal[:]
        for i in range(len(goal)):
            if goal[i] is not None:
                goal[i] = goal[i] / 180.0 * 3.141592654
        self.moveit2.move_to_configuration(goal)

        ret = self.moveit2.wait_until_executed()
        if ret:
            self.move_success = True
        else:
            self.move_success = False
        self.move_down = True

    def terminate(self, new_status):
        if not self.static_goal:
            self.goal = None


class ArmMoveToName(py_trees.behaviour.Behaviour):
    def __init__(self, name, moveit_config: dict, goal=None, goal_name=None):
        super(ArmMoveToName, self).__init__(name)
        self.bb = self.attach_blackboard_client()
        self.moveit_config = moveit_config
        self.group_name = self.moveit_config["group_name"]
        self.base_link_name = self.moveit_config["base_link_name"]
        self.joint_names = self.moveit_config["joint_names"]
        self.end_effector_name = self.moveit_config["end_effector_name"]
        self.planner_id = self.moveit_config["planner_id"]
        self.max_velocity = self.moveit_config["max_velocity"]
        self.max_acceleration = self.moveit_config["max_acceleration"]
        self.goal_name = goal_name
        self.goal = goal
        if goal is None:
            self.static_goal = False
        else:
            self.static_goal = True
        self.move_down = False
        self.move_success = False

    def setup(self, **kwargs):
        self.node = kwargs["node"]
        # init pymoveit2, please read pymoveit2 docs and codes
        self.moveit2 = MoveIt2(
            node=self.node,
            joint_names=self.joint_names,
            base_link_name=self.base_link_name,
            end_effector_name=self.end_effector_name,
            group_name=self.group_name,
        )
        self.moveit2.planner_id = self.planner_id
        self.moveit2.max_velocity = self.max_velocity
        self.moveit2.max_acceleration = self.max_acceleration

    def initialise(self):
        if self.static_goal:
            return
        if not self.goal_name:
            return
        try:
            self.goal = self.bb.get(self.goal_name)
            self.bb.set(self.goal_name, None)
        except Exception as e:
            self.logger.error("[Ex] get xarm_target_pose exception: {}".format(e))

    def update(self):
        if self.goal is None:
            return py_trees.common.Status.FAILURE
        if not "named_poses" in self.moveit_config:
            return py_trees.common.Status.FAILURE
        if not self.goal in self.moveit_config["named_poses"]:
            return py_trees.common.Status.FAILURE
        if self.status != py_trees.common.Status.RUNNING:
            self.move_down = False
            self.move_success = False
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
        while self.status != py_trees.common.Status.RUNNING:
            time.sleep(0.1)

        # wait for joints state
        while self.moveit2.joint_state is None:
            time.sleep(0.1)

        goal = self.moveit_config["named_poses"][self.goal][:]
        for i in range(len(goal)):
            if goal[i] is not None:
                goal[i] = goal[i] / 180.0 * 3.141592654

        self.moveit2.move_to_configuration(goal)

        ret = self.moveit2.wait_until_executed()
        self.logger.info("move down {}".format(ret))
        if ret:
            self.move_success = True
        else:
            self.move_success = False
        self.move_down = True

    def terminate(self, new_status):
        if not self.static_goal:
            self.goal = None
