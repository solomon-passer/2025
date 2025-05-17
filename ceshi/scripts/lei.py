#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
from turtlesim.srv import SetPen, TeleportAbsolute  # 修改：使用SetPen服务

def draw_circle():
    # 初始化节点
    rospy.init_node('draw_circle', anonymous=True)
    velocity_publisher = rospy.Publisher('/turtle1/cmd_vel', Twist, queue_size=10)
    rate = rospy.Rate(10)  # 10Hz

    # 设置初始颜色
    rospy.wait_for_service('/turtle1/set_pen')  # 修改：服务名称改为set_pen
    try:
        set_pen = rospy.ServiceProxy('/turtle1/set_pen', SetPen)  # 修改：使用SetPen服务
        # 修改：SetPen服务的参数为(r, g, b, width, off)
        set_pen(255, 0, 0, 10, 0)  # 设置为红色，线宽2，启用画笔
    except rospy.ServiceException as e:
        print(f"Service call failed: {e}")

    # 定义移动指令
    vel_msg = Twist()
    vel_msg.linear.x = 4.0
    vel_msg.angular.z = 1.0

    # 移动并绘制圆环
    for _ in range(360):
        velocity_publisher.publish(vel_msg)
        rate.sleep()

    # 停止乌龟
    vel_msg.linear.x = 0
    vel_msg.angular.z = 0
    velocity_publisher.publish(vel_msg)

if __name__ == '__main__':
    try:
        draw_circle()
    except rospy.ROSInterruptException:
        pass
