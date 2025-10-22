## 0913
- 在运行代码之前需要先运行：bash /opt/apps/roboticsservice/runService.sh 用来打开接收pico遥操作的接收
- 将pico和电脑之间的通讯打通，moveit2的接口也已经接入程序中，可以正常进行编译

## 0923
- 桌面快捷控制应用中的启动介绍在文件：file:///home/rpp/rpp_tools/starterbutton/.starterButton/config/config.json

## 0925
- 运行的包为：ros2 run pico_kinova pico_kinova
- 在运行之前尽量先开启rviz，可以看到机械臂的运动情况
- 由于cmake文件是在之前的基础上进行修改的，即使有一些依赖不会用到也不要删除，不然可能会编译出错（除非已经确定不会在cmake文件和运行文件中调用的）
- 在utilities.cpp文件中修改机械臂的ip地址

## 0927
- 添加matplotlib绘图功能，可以绘制二维和三维的图表
- matplotlib只支持2.0以下版本的numpy
- cmakelists中需要手动包含numpy的头文件
- conda中的python也需要安装cmake，make，gcc，c++，才可以征程用于编译过程，不然会发生版本错误
- conda install cmake make gcc g++

## 代码编译
- 编译所有的程序包
- colcon build 
- 编译指定的程序包
- colcon build --packages-select pico_kinova --mixin debug
- source环境
- source /opt/ros/humble/setup.bash  
- source install/local_setup.bash
- 清除之前的构建
- rm -rf build install log
- 列出所有包及其内容
- colcon list --packages-select pico_kinova
- 查看tf树
- ros2 run rqt_tf_tree rqt_tf_tree

- 头显传输过来的数据如下
{
    "functionName": "Tracking",
    "value": 
        "{\"predictTime\":521697731.067,
        \"appState\":{\"focus\":true},
        \"Head\":{\"pose\":\"-0.005365652,-0.0509465337,-0.02424921,-0.0219538137,-0.125764042,0.0502647348,-0.990542769\",\"status\":3},
        \"Controller\":
            {\"left\":
                {\"axisX\":0.0,\"axisY\":0.0,\"axisClick\":false,\"grip\":0.0,\"trigger\":0.0,\"primaryButton\":false,\"secondaryButton\":false,\"menuButton\":false,\"pose\":\"-0.108875774,-0.406339616,-0.232757375,-0.07052495,-0.170688525,-0.15705815,0.9701672\"},
            \"right\":
                {\"axisX\":0.0,\"axisY\":0.0,\"axisClick\":false,\"grip\":0.0,\"trigger\":0.505882382392883,\"primaryButton\":false,\"secondaryButton\":false,\"menuButton\":false,\"pose\":\"0.0470392,-0.285911828,-0.182648584,0.100988381,0.185609534,-0.018177418,0.9772513\"}},
        \"timeStampNs\":1758800748545324032,
        \"Input\":1}"
}
