#include<ros/ros.h> 
#include<std_msgs/String.h> 
//void chao_callback(std_msgs::String msg) {ROS_INFO(msg.data.c_str());}
void doMsg(const std_msgs::String::ConstPtr& msg_p)
{//ConstPtr智能指针类型，&引用
    ROS_INFO("我听见:%s",msg_p->data.c_str());
}
int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");//zh_CN.UTF-8如果英文版ub
    ros::init(argc,argv,"ma_node");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("fang",10,doMsg);
 
    //ros::spin();代替下面
    while(ros::ok())
    {   ros::spinOnce();//处理回调函数，官方
    }return 0;
}