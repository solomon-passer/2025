#include"ros/ros.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"//2和3结合使用，缓存作用
#include "geometry_msgs/PointStamped.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
//包含transform函数
int main(int argc, char  *argv[])
{
    setlocale(LC_ALL,"");
    ROS_INFO("这是订阅方");
    ros::init(argc,argv,"static_sub");
    ros::NodeHandle nh;
  
    tf2_ros::Buffer buffer;  //Buffer缓存
    tf2_ros::TransformListener listener(buffer);
    geometry_msgs::PointStamped ps;
    //在雷达坐标系中障碍物的坐标
    ps.header.frame_id = "laser";
    ps.header.stamp = ros::Time::now();
    ps.point.x = 2.0;
    ps.point.y = 3.0;
    ps.point.z = 5.0;
    ros::Rate rate(1);
    ros::Duration(2).sleep();//因为订阅要耗时
    //转换算法，需调用TF内置实现
    while (ros::ok())
    {   /*核心：将ps转换为相对于base_link的坐标点
        调用了buffer的转换函数transform
        参数1被转换的坐标点，参数2目标坐标系
        需要包含头文件5*/
       try
       {
        geometry_msgs::PointStamped ps_out;
        ps_out=buffer.transform(ps,"base_link");

        ROS_INFO("转换后的数据:(%.2f,%.2f,%.2f),参考的坐标系是:%s",
            ps_out.point.x,
            ps_out.point.y,
            ps_out.point.z,
            ps_out.header.frame_id.c_str()
        );
       }
       catch(const std::exception& e)//异常处理
       {
        std::cerr << e.what() << '\n';
        ROS_INFO("异常消息：%s,e.what()");
       }
       
        rate.sleep();  
        ros::spinOnce();
    }
    return 0;
}
