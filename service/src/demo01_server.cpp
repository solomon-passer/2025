#include"ros/ros.h"
#include"fuwu/addints.h"

bool donums(fuwu::addints::Request &req, fuwu::addints::Response &res)
{
    int num1=req.num1;
    int num2=req.num2;
    int sum=num1+num2;

    res.sum=sum;
    ROS_INFO("结果sum=%d",sum); 
    return true;
}


int main(int argc, char  *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc, argv, "fuwuduan");
    ros::NodeHandle nh;  
    ros::ServiceServer sever= nh.advertiseService("addints",donums);
    ROS_INFO("服务端已启动，等待客户端请求");

    ros::spin();
    return 0;
}
