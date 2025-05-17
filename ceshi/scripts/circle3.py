#!/usr/bin/env python
# -*- coding: utf-8 -*-
import rospy
from geometry_msgs.msg import Twist
from turtlesim.srv import SetPen, TeleportAbsolute
from std_srvs.srv import Empty
import math
import random

def draw_concentric_disks():
    rospy.init_node('draw_concentric_disks', anonymous=True)

    # 等待服务可用
    rospy.wait_for_service('turtle1/set_pen')
    rospy.wait_for_service('turtle1/teleport_absolute')
    rospy.wait_for_service('clear')
    
    set_pen = rospy.ServiceProxy('turtle1/set_pen', SetPen)
    teleport_abs = rospy.ServiceProxy('turtle1/teleport_absolute', TeleportAbsolute)
    clear_bg = rospy.ServiceProxy('clear', Empty)
    
    cmd_vel_pub = rospy.Publisher('turtle1/cmd_vel', Twist, queue_size=10)

    rospy.sleep(1)  # 等待连接完成

    # turtlesim中心点（圆心） - 恢复默认值
    center_x = 10
    center_y = 10

    # 圆盘半径 - 调整为8个圆
    radii = [4.0, 3.5, 3.0, 2.5, 2.0, 1.5, 1, 0.5]
    
    # 颜色列表
    colors = [
        (255, 0, 0),     # 红
        (0, 255, 0),     # 绿
        (0, 0, 255),     # 蓝
        (255, 255, 0),   # 黄
        (0, 255, 255),   # 青
        (255, 165, 0),   # 橙
        (128, 0, 128),   # 紫
        (255, 192, 203), # 粉
    ]
 
    angular_speed = 10.0   # 乌龟转速
    rate = rospy.Rate(50)  # 控制发布频率

    
    initial_width = 255  # 初始宽度
    min_width = 30       # 设置最小宽度为30

    # 无限循环绘制同心圆
    while not rospy.is_shutdown():
        clear_bg()  # 清屏
        
        # 每次循环前随机打乱颜色顺序
        random_colors = random.sample(colors, len(colors))
        
        # 第一个圆的宽度固定为255
        current_width = initial_width

        for idx, radius in enumerate(radii):
            if idx > 0:  # 从第二个圆开始才减少宽度
                # 动态调整随机减少值的范围
                min_decrement = 25
                max_decrement = 45
                
                # 计算剩余需要绘制的圆数（不包括第一个圆）
                remaining_circles = len(radii) - idx
                
                # 当剩余圆数较多但宽度接近下限时，减小最大减少值
                if current_width - (min_decrement * remaining_circles) < min_width:
                    max_decrement = min(max_decrement, current_width - min_width)
                
                # 确保最大减少值不小于最小减少值
                if max_decrement < min_decrement:
                    max_decrement = min_decrement
                
                # 随机生成减少值
                width_decrement = random.randint(min_decrement, max_decrement)
                
                # 计算当前宽度，确保不小于min_width
                current_width = max(min_width, current_width - width_decrement)
            else:
                width_decrement = 0  # 第一个圆使用初始宽度
                
            rospy.loginfo(f"Drawing circle {idx+1}/{len(radii)} with width: {current_width}, decrement: {width_decrement}")
            
            r, g, b = random_colors[idx]  # 使用随机化的颜色顺序

            # 计算线速度（v = R * w）
            linear_speed = radius * angular_speed

            # 关闭画笔并移动到起点（圆的最右端）
            set_pen(r, g, b, current_width, 1)
            start_x = center_x + radius
            start_y = center_y
            teleport_abs(start_x, start_y, math.pi/2)

            # 打开画笔，准备绘制
            set_pen(r, g, b, current_width, 0)

            # 设置速度指令
            twist = Twist()
            twist.linear.x = linear_speed
            twist.angular.z = angular_speed

            # 计算绘制一整圈所需时间
            duration = 2 * math.pi * radius / linear_speed
            start_time = rospy.Time.now()

            # 开始画圆
            while rospy.Time.now() - start_time < rospy.Duration(duration) and not rospy.is_shutdown():
                cmd_vel_pub.publish(twist)
                rate.sleep()

            # 停止乌龟移动
            twist.linear.x = 0
            twist.angular.z = 0
            cmd_vel_pub.publish(twist)

            # 关闭画笔，准备下一个圆
            set_pen(r, g, b, current_width, 1)

        # 结束后回到圆心
        teleport_abs(center_x, center_y, 0.0)
        rospy.loginfo("Finished drawing 8 concentric color disks. Clearing and restarting...")
        
        # 短暂延迟后重新开始
        rospy.sleep(0.2)

if __name__ == '__main__':
    try:
        draw_concentric_disks()
    except rospy.ROSInterruptException:
        rospy.loginfo("Program interrupted by user")
