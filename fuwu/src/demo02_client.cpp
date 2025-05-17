#include"ros/ros.h"
#include"fuwu/addints.h"


int main(int argc, char  *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc, argv, "dabao");
    ros::NodeHandle nh;  
    ros::ServiceClient client= nh.serviceClient<fuwu::addints>("addints");
    ROS_INFO("开始处理");

    fuwu::addints aa;
    aa.request.num1=10;
    aa.request.num2=20;
    /* 处理响应 */
    bool flag = client.call(aa);
    if(flag)
    {
        ROS_INFO("请求成功，结果是：%d",aa.response.sum);
    }
    else
    {
        ROS_INFO("请求失败");
    }
}
