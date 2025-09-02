import py_trees
import threading
from pymoveit2 import GripperInterface


class MoveitGripper(py_trees.behaviour.Behaviour):
    def __init__(self, name, moveit_config: dict, goal=""):
        super(MoveitGripper, self).__init__(name)
        self.moveit_config = moveit_config
        self.group_name = self.moveit_config["group_name"]
        self.joint_names = self.moveit_config["joint_names"]
        self.open_joint_positions = self.moveit_config["open_joint_positions"]
        self.close_joint_positions = self.moveit_config["close_joint_positions"]
        self.action_name = self.moveit_config["action_name"]
        self.goal = goal
        self.move_down = False
        self.move_success = False

    def setup(self, **kwargs):
        self.node = kwargs["node"]
        # init pymoveit2, please read pymoveit2 docs and codes
        self.gripper = GripperInterface(
            node=self.node,
            gripper_joint_names=self.joint_names,
            open_gripper_joint_positions=self.open_joint_positions,
            closed_gripper_joint_positions=self.close_joint_positions,
            gripper_group_name=self.group_name,
            gripper_command_action_name=self.action_name,
        )

    def initialise(self):
        pass

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
        if self.goal == "open":
            self.gripper.open()
        elif self.goal == "close":
            self.gripper.close()
        else:
            self.move_success = False
            self.move_down = True
        ret = self.gripper.wait_until_executed()
        self.logger.info("move down {}".format(ret))
        if ret:
            self.move_success = True
        else:
            self.move_success = False
        self.move_down = True

    def terminate(self, new_status):
        pass
