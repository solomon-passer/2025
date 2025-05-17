#include"ros/ros.h"
#include"fuwu/addints.h"


int main(int argc, char  *argv[])
{
    setlocale(LC_ALL,"");
    if (argc != 3)
    //(0-文件路径 1传入的参数 2传入的参数 3节点名称 4日志路径)
    {
        ROS_ERROR("请提交两个整数");
        return 1;
    }
    ros::init(argc, argv, "dabao");
    ros::NodeHandle nh;  
    ros::ServiceClient client= nh.serviceClient<fuwu::addints>("addints");
    ROS_INFO("开始处理");

    fuwu::addints aa;
    aa.request.num1=atoi(argv[1]);
    aa.request.num2=atoi(argv[2]);
   /* 解决因为先启动导致的报错,先挂起 */
   client.waitForExistence();
    //ros::service::waitForService("addints");
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
