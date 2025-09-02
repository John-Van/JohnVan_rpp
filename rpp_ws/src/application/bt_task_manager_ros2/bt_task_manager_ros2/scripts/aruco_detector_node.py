#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
import rclpy.wait_for_message
from sensor_msgs.msg import Image, CameraInfo
from cv_bridge import CvBridge, CvBridgeError
from std_msgs.msg import Bool
from aruco_msgs.msg import Marker, MarkerArray
import numpy as np
import tf_transformations
from tf2_ros import TransformBroadcaster
from geometry_msgs.msg import TransformStamped
import cv2
import time
import threading


class ArUcoDetector(Node):
    def __init__(self):
        super().__init__("aruco_detector_node")

        # 参数
        self.declare_parameter("camera_info_topic_name", "/kortex_camera/color/camera_info")
        self.declare_parameter("image_topic_name", "/kortex_camera/color/image_raw")
        self.declare_parameter("camera_frame", "kortex_camera_color_frame")
        self.declare_parameter("marker_size", 0.08)
        self.declare_parameter("visualize", True)

        self.camera_info_topic_name = self.get_parameter("camera_info_topic_name").get_parameter_value().string_value
        self.image_topic_name = self.get_parameter("image_topic_name").get_parameter_value().string_value
        self.camera_frame = self.get_parameter("camera_frame").get_parameter_value().string_value
        self.marker_size = self.get_parameter("marker_size").get_parameter_value().double_value
        self.visualize = self.get_parameter("visualize").get_parameter_value().bool_value

        self.camera_matrix = None
        self.dist_coeffs = None
        self.img_que = []
        self.lock = threading.Lock()  # 用于保护队列的访问

        # CvBridge和TF2广播器
        self.bridge = CvBridge()
        self.tf_broadcaster = TransformBroadcaster(self)

        # 订阅器和发布器
        self.image_sub = self.create_subscription(Image, self.image_topic_name, self.image_callback, 10)
        self.camera_info_sub = self.create_subscription(CameraInfo, self.camera_info_topic_name, self.camera_info_callback, 10)

        self.control_sub = self.create_subscription(Bool, "/aruco_detector/start", self.start_processing, 10)
        self.marker_pub = self.create_publisher(MarkerArray, "/aruco_detector/markers", 10)
        self.image_pub = self.create_publisher(Image, "/aruco_detector/result", 10)

        # 标志位和处理线程
        self.processing_flag = False
        self.process_thread = threading.Thread(target=self.process_queue)
        self.process_thread.daemon = True
        self.process_thread.start()

        self.get_logger().info("ArUco Detector initialized.")
        self.processing_flag = True

    def start_processing(self, msg):
        if msg.data:
            self.processing_flag = True
            self.get_logger().info("Start processing.")
        else:
            self.get_logger().info("Stop processing.")
            self.processing_flag = False
            with self.lock:
                self.img_que = []  # 清空队列

    def camera_info_callback(self, camera_info):
        self.camera_matrix = np.array(camera_info.k).reshape((3, 3))
        self.dist_coeffs = np.array(camera_info.d)
        self.get_logger().info("Camera information received.")
        self.destroy_subscription(self.camera_info_sub)
        self.camera_info_sub = None

    def image_callback(self, data):
        if not self.processing_flag:
            return
        try:
            cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
        except CvBridgeError as e:
            self.get_logger().error(str(e))
            return
        with self.lock:  # 确保线程安全
            if len(self.img_que) > 10:
                self.img_que.pop(0)
            self.img_que.append(cv_image)

    def process_queue(self):
        while rclpy.ok():
            if not self.processing_flag or not self.img_que:
                time.sleep(0.1)  # 暂停以降低CPU占用
                continue

            with self.lock:  # 确保线程安全
                if not self.img_que:
                    continue
                cv_image = self.img_que.pop(0)

            markers, image = self.detect_aruco(cv_image)

            if markers is not None:
                marker_array = MarkerArray()
                for marker in markers:
                    id, _, pose = marker
                    marker_msg = Marker()
                    marker_msg.id = int(id)
                    marker_msg.pose.pose.position.x = pose[0]
                    marker_msg.pose.pose.position.y = pose[1]
                    marker_msg.pose.pose.position.z = pose[2]
                    quat = tf_transformations.quaternion_from_euler(pose[3], pose[4], pose[5])
                    marker_msg.pose.pose.orientation.x = quat[0]
                    marker_msg.pose.pose.orientation.y = quat[1]
                    marker_msg.pose.pose.orientation.z = quat[2]
                    marker_msg.pose.pose.orientation.w = quat[3]
                    marker_array.markers.append(marker_msg)

                    self.publish_marker_tf(id, pose)

                self.marker_pub.publish(marker_array)

                if self.visualize:
                    ros_image = self.bridge.cv2_to_imgmsg(image, "bgr8")
                    self.image_pub.publish(ros_image)

    def detect_aruco(self, image):
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        dictionary = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_ARUCO_ORIGINAL)
        parameters = cv2.aruco.DetectorParameters()
        corners, ids, _ = cv2.aruco.detectMarkers(gray, dictionary, parameters=parameters)

        if ids is not None:
            markers = []
            for i in range(len(ids)):
                rvec, tvec, _ = cv2.aruco.estimatePoseSingleMarkers(corners, self.marker_size, self.camera_matrix, self.dist_coeffs)
                rot_matrix, _ = cv2.Rodrigues(rvec[i])
                euler_angles = np.array(cv2.RQDecomp3x3(rot_matrix)[0])
                markers.append((ids[i][0], corners[i], (tvec[i][0][0], tvec[i][0][1], tvec[i][0][2], *euler_angles)))
                image = cv2.drawFrameAxes(image, self.camera_matrix, self.dist_coeffs, rvec[i], tvec[i], 0.1)
                image = cv2.aruco.drawDetectedMarkers(image, corners, ids)
            return markers, image
        return None, None

    def publish_marker_tf(self, marker_id, pose):
        transform = TransformStamped()
        transform.header.stamp = self.get_clock().now().to_msg()
        transform.header.frame_id = self.camera_frame
        transform.child_frame_id = f"marker_{marker_id}_link"
        transform.transform.translation.x = pose[0]
        transform.transform.translation.y = pose[1]
        transform.transform.translation.z = pose[2]
        quat = tf_transformations.quaternion_from_euler(np.radians(pose[3]), np.radians(pose[4]), np.radians(pose[5]))
        transform.transform.rotation.x = quat[0]
        transform.transform.rotation.y = quat[1]
        transform.transform.rotation.z = quat[2]
        transform.transform.rotation.w = quat[3]
        self.tf_broadcaster.sendTransform(transform)


def main(args=None):
    rclpy.init(args=args)
    node = ArUcoDetector()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
