#include<ros/ros.h>  
#include<std_msgs/String.h> 
#include<sstream>
int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc, argv, "chao_node");//初始化ROS节点，设置节点名称为"chao_node"
    printf("我的枪去而复返\n");

    ros::NodeHandle nh;  //nh这是与ROS系统通信的接口
    ros::Publisher pub = nh.advertise<std_msgs::String>("fang", 10);  
    // 创建一个发布者pub，用于发布类型为std_msgs::String的消息到话题"fang"，队列大小为10
    std_msgs::String msg;
    ros::Rate loop_rate(10);//频率为10Hz，0.1s。rate
    int count=0;
    ros::Duration(3.0).sleep();//休眠
    while(ros::ok())  // 当ROS运行正常时（没有接收到关闭信号），继续循环
    {
        count++;
        std::stringstream ss;//开始拼接数据
        ss << "hihi-->" << count;
        msg.data = ss.str();//string提取为字符串
        pub.publish(msg);
        //添加日志
        ROS_INFO("发送的数据是:%s",msg.data.c_str());//c风格str
        loop_rate.sleep();//rate.sleep();
        ros::spinOnce();
    }return 0;  
}