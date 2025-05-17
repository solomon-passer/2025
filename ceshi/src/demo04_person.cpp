#include<ros/ros.h>
#include<ceshi/Person.h>

void doperson(const ceshi::Person::ConstPtr& a)
{
    ROS_INFO("订阅信息：%s,%d,%.2f",a->name.c_str(),a->age,a->height);
    //c_str理解：string转字符串
}

int main(int argc, char  *argv[])
{
    setlocale(LC_ALL,"");
    ROS_INFO("这是订阅方");
    ros::init(argc, argv, "jiazhang");

    ros::NodeHandle nh;
    ros::Subscriber sub=nh.subscribe("liaotian",10,doperson);
    ros::spin();
    //无限循环，持续检查消息队列并调用所有待处理的回调函数。

    return 0;
}
