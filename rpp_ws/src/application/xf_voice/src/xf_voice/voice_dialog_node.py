#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
import requests
import time
import hashlib
import base64
import json
from std_msgs.msg import String, Int8

class VoiceDialogNode(Node):
    def __init__(self):
        super().__init__('voice_dialog_node')
        
        # 讯飞API配置（替换为你的账号信息）
        self.declare_parameter('app_id', 'e1e3f3c1')
        self.declare_parameter('api_key', 'beb445748fdaabde7258cbb07a044d34')
        self.declare_parameter('api_secret', 'MGY0MjEwMDg2ZGQ4NzljZTdkZjg3MmQw')
        
        self.app_id = self.get_parameter('app_id').value
        self.api_key = self.get_parameter('api_key').value
        self.api_secret = self.get_parameter('api_secret').value
        
        # 讯飞闲聊API地址（根据需要替换为其他NLP接口）
        self.nlp_url = "https://spark-api-open.xf-yun.com/v1/chat/completions"
        
        # 状态管理
        self.in_dialog = False  # 是否在对话中
        self.dialog_context = []  # 对话上下文
        
        # 订阅语音识别结果
        self.iat_sub = self.create_subscription(
            String,
            '/xf/voice',
            self.iat_callback,
            10)
        
        # 发布TTS文本
        self.tts_pub = self.create_publisher(
            String,
            '/tts_topic',
            10)
        
        # 唤醒信号订阅
        self.wakeup_sub = self.create_subscription(
            Int8,
            '/awake_flag',
            self.wakeup_callback,
            10)
        
        self.get_logger().info('语音对话节点已启动')

    def get_auth_headers(self):
        """生成讯飞API鉴权头部"""
        cur_time = str(int(time.time()))
        signa = hashlib.md5(
            (self.api_key + cur_time + self.api_secret).encode('utf-8')
        ).hexdigest()
        
        return {
            'X-Appid': self.app_id,
            'X-CurTime': cur_time,
            'X-Param': base64.b64encode(
                json.dumps({"scene": "main"}).encode('utf-8')
            ),
            'X-CheckSum': signa,
            'Content-Type': 'application/x-www-form-urlencoded; charset=utf-8'
        }

    def call_xf_nlp_api(self, text):
        """调用讯飞自然语言处理API"""
        try:
            # 保留最近3轮对话作为上下文
            self.dialog_context.append({"role": "user", "content": text})
            if len(self.dialog_context) > 6:  # 3轮对话，每轮包含用户和AI
                self.dialog_context = self.dialog_context[-6:]
            
            headers = self.get_auth_headers()
            data = {'text': text}
            
            response = requests.post(self.nlp_url, headers=headers, data=data)
            result = json.loads(response.text)
            
            if result.get('code') == 0:
                answer = result.get('data', {}).get('answer', '抱歉，我没理解你的意思')
                self.dialog_context.append({"role": "ai", "content": answer})
                return answer
            else:
                error_msg = f"API错误: {result.get('message', '未知错误')}"
                self.get_logger().error(error_msg)
                return "抱歉，处理你的请求时出错了"
                
        except Exception as e:
            self.get_logger().error(f"调用API失败: {str(e)}")
            return "抱歉，连接服务器失败"

    def wakeup_callback(self, msg):
        """处理唤醒信号"""
        if msg.data == 1 and not self.in_dialog:
            self.in_dialog = True
            self.get_logger().info("收到唤醒信号，开始对话")
            self.tts_pub.publish(String(data="我在，有什么可以帮你？"))

    def iat_callback(self, msg):
        """处理语音识别结果"""
        if not self.in_dialog:
            return
            
        user_text = msg.data.strip()
        if not user_text:
            return
            
        self.get_logger().info(f"识别到: {user_text}")
        
        # 检查退出对话指令
        if user_text in ['退出', '结束对话', '再见']:
            self.tts_pub.publish(String(data="再见，有需要再叫我"))
            self.in_dialog = False
            self.dialog_context = []
            return
            
        # 调用讯飞API处理
        self.get_logger().info("正在处理...")
        ai_response = self.call_xf_nlp_api(user_text)
        
        # 语音播报结果
        self.get_logger().info(f"回复: {ai_response}")
        self.tts_pub.publish(String(data=ai_response))

def main(args=None):
    rclpy.init(args=args)
    node = VoiceDialogNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
    