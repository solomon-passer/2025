#include"ros/ros.h"
#include"tf2_ros/static_transform_broadcaster.h"
#include"geometry_msgs/TransformStamped.h"
#include"tf2/LinearMath/Quaternion.h"

int main(int argc, char  *argv[])
{
    setlocale(LC_ALL,"");
    ROS_INFO("这是发布方");
    ros::init(argc, argv, "static_pub");
    ros::NodeHandle nh;//非必须

    tf2_ros::StaticTransformBroadcaster pub;//发布话题名
    //4.组织被发布的数据
    geometry_msgs::TransformStamped tfs;
    tfs.header.stamp=ros::Time::now();//时间戳
    tfs.header.frame_id="base_link";//父
    tfs.child_frame_id="laser";//子
    tfs.transform.translation.x=0.2;
    tfs.transform.translation.y=0.0;
    tfs.transform.translation.z=0.5;
    //根据欧拉角转换
    tf2::Quaternion qtn;//创建四元数对象
    //向对象设置欧拉角，这个对象可以进行转换
    //翻滚，俯仰，偏航
    qtn.setRPY(0,0,0);//弧度制
    tfs.transform.rotation.x= qtn.getX();
    tfs.transform.rotation.y= qtn.getY();
    tfs.transform.rotation.z= qtn.getZ();
    tfs.transform.rotation.w= qtn.getW();
    //5.发布数据
    pub.sendTransform(tfs);
    ros::spin();
    return 0;

}
