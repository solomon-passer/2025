#include<ros/ros.h>  
#include<ceshi/Person.h>//前面生产的.h文件
int main(int argc, char  *argv[])
{
    setlocale(LC_ALL,"");
    ROS_INFO("这是发布方");
    ros::init(argc, argv, "banzhuren");
    //供了一个接口，允许节点访问 ROS 的各种功能
    ros::NodeHandle nh;
    //等号右边用来创建一个发布者对象的函数
    ros::Publisher pub = nh.advertise<ceshi::Person>("liaotian", 10);

    ceshi::Person a;
    a.name="奥特曼";
    a.age=18;
    a.height=183;
    ros::Rate rate(1);
    while(ros::ok())
    {
        
        //数据发布
        pub.publish(a);
        ROS_INFO("发布的消息:%s,%d,%.2f",a.name.c_str(),a.age,a.height);
        rate.sleep();//让频率生效
        ros::spinOnce;
        a.age+=1;//修改发布数据
    }
    return 0;
}
