import py_trees

import rclpy
import tf_transformations as tft
import numpy as np
import math

from geometry_msgs.msg import TransformStamped
from tf2_ros import TransformException
from tf2_ros import Buffer
from tf2_ros import TransformListener
from tf2_ros import TransformBroadcaster


class GetGraspPoseByTF(py_trees.behaviour.Behaviour):
    def __init__(
        self,
        name,
        arm_base_frame="base_link",
        target_frame="",
        grasp_frame="grasp_pose",
        feed_depth=0.1,
        offset_x=0,
        offset_y=0,
        offset_z=0,
        use_target_orient=False,
        grasp_angles=[90, 0, 90],
    ):
        super(GetGraspPoseByTF, self).__init__(name)
        self.bb = self.attach_blackboard_client()
        self.arm_base_frame = arm_base_frame
        self.target_frame = target_frame
        self.grasp_frame = grasp_frame
        self.feed_depth = feed_depth
        self.offset_x = offset_x
        self.offset_y = offset_y
        self.offset_z = offset_z
        self.use_target_orient = use_target_orient
        self.grasp_angles = np.radians(grasp_angles)

    def setup(self, **kwargs):
        self.node = kwargs["node"]
        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self.node)
        self.tf_broadcaster = TransformBroadcaster(self.node)

    def initialise(self):
        self.stable_count = 0
        self.last_translation = None
        self.last_rotation = None
        self.bb.register_key(self.grasp_frame, access=py_trees.common.Access.WRITE)

    def update(self) -> py_trees.common.Status:

        try:
            t = self.tf_buffer.lookup_transform(self.arm_base_frame, self.target_frame, rclpy.time.Time())
        except TransformException as ex:
            self.logger.error("tf error. {}".format(ex))
            self.stable_count = 0
            self.last_translation = None
            self.last_rotation = None
            return py_trees.common.Status.RUNNING

        translation = t.transform.translation
        rotation = t.transform.rotation

        # get first transform
        if self.last_translation is None:
            self.last_translation = translation
            self.last_rotation = rotation
            return py_trees.common.Status.RUNNING

        # in this condition check, requires the tf is close to the "first" tf, if tf is change too much, reset the "first" tf
        if (
            math.fabs(translation.x - self.last_translation.x) > 0.01
            or math.fabs(translation.y - self.last_translation.y) > 0.01
            or math.fabs(translation.z - self.last_translation.z) > 0.01
        ):
            self.last_translation = translation
            self.last_rotation = rotation
            return py_trees.common.Status.RUNNING

        self.stable_count += 1
        if self.stable_count < 10:
            return py_trees.common.Status.RUNNING

        # use newest transform to get grasp pose
        if self.use_target_orient:
            target_trans = tft.translation_matrix([translation.x, translation.y, translation.z])
            target_orient = tft.quaternion_matrix([rotation.x, rotation.y, rotation.z, rotation.w])
            grasp_orient = np.dot(target_orient, tft.euler_matrix(math.pi, 0, 0))  # reserve 180 degree
            delta_trans = tft.translation_matrix([self.offset_x, self.offset_y, self.offset_z - self.feed_depth])
            grasp_transform = np.dot(target_trans, grasp_orient)
            grasp_transform = np.dot(grasp_transform, delta_trans)  # offset in tcp coordinate
        else:
            target_trans = tft.translation_matrix([translation.x, translation.y, translation.z])
            grasp_orient = tft.euler_matrix(self.grasp_angles[0], self.grasp_angles[1], self.grasp_angles[2])
            delta_trans = tft.translation_matrix([self.offset_x, self.offset_y, self.offset_z - self.feed_depth])
            grasp_transform = np.dot(target_trans, grasp_orient)
            grasp_transform = np.dot(grasp_transform, delta_trans)  # offset in tcp coordinate

        translate = tft.translation_from_matrix(grasp_transform)
        angles = tft.euler_from_matrix(grasp_transform)
        angles = [math.degrees(a) for a in angles]
        quat = tft.quaternion_from_matrix(grasp_transform)

        self.logger.info("target {} {}".format(translate, angles))
        self.bb.set(self.grasp_frame, list(translate) + list(angles), overwrite=True)

        t = TransformStamped()
        t.header.stamp = self.node.get_clock().now().to_msg()
        t.header.frame_id = self.arm_base_frame
        t.child_frame_id = self.grasp_frame
        t.transform.translation.x = translate[0]
        t.transform.translation.y = translate[1]
        t.transform.translation.z = translate[2]
        t.transform.rotation.x = quat[0]
        t.transform.rotation.y = quat[1]
        t.transform.rotation.z = quat[2]
        t.transform.rotation.w = quat[3]
        self.tf_broadcaster.sendTransform(t)

        return py_trees.common.Status.SUCCESS

    def terminate(self, new_status):
        pass
