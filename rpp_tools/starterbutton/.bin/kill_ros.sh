#! /bin/bash
source ~/.rpprc

# 获取所有正在运行的节点
nodes=$(ros2 node list)

# 检查是否有节点正在运行
if [ -z "$nodes" ]; then
  echo "没有节点活跃"
  exit 1
fi

# 遍历每个节点
for node in $nodes
do
  echo "查找到节点: $node"

  # 使用 pgrep 查找 ROS2 节点进程的 PID
  node_pid=$(pgrep -f "$node")

  # 检查是否找到了 PID
  if [ -z "$node_pid" ]; then
    echo "pid查找失败: $node"
    continue
  fi

  # 终止进程
  kill $node_pid

  # 检查 kill 是否成功
  if [ $? -eq 0 ]; then
    echo "成功关闭节点: $node (PID: $node_pid)"
  else
    echo "关闭节点失败: $node"
  fi
done
