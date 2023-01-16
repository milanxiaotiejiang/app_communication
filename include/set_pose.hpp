//#include "ros/ros.h"
//#include "std_msgs/String.h"
//#include "geometry_msgs/PoseWithCovarianceStamped.h"
//#include "geometry_msgs/Twist.h"
//#include "math.h"
//#include "tf/transform_listener.h"
//
//#define PI 3.1415926
//
//ros::Publisher cmd_pub;
//ros::Publisher initial_pose_pub;
//tf::TransformListener tf_listener;
//
//
//class Initial_Pose
//{
//  public:
//    Initial_Pose(ros::NodeHandle);
//    ~Initial_Pose();
//    double get_odom_angle();
//    void rotate_circle();
//    void publish_initial_pos();
//    double normalize_angle(double angle);
//    double alpha = 0.0;//radian value
//    double x_pos = 0.0;
//    double y_pos = 0.0;
//  private:
//    ros::NodeHandle nh;
//
//    double last_angle;//上个循环旋转角度
//    double current_angle;//当前旋转角度
//    double rotate_angle = 360.0;//目标旋转角度360度
//    double turn_angle = 0;//已经旋转角度
//    double tolerance = 4.0; //自旋容许误差角度
//
//
//};
//
//Initial_Pose::Initial_Pose(ros::NodeHandle nh)
//{
//  cmd_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
//  initial_pose_pub = nh.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose", 10);
//  tf::TransformListener tf_listener;
//}
//
//Initial_Pose::~Initial_Pose()
//{
//
//}
//
//double Initial_Pose::get_odom_angle()
//{
//    try
//    {
//        tf::StampedTransform stamped_transform;
//        tf_listener.waitForTransform("odom", "base_link", ros::Time(5.0), ros::Duration(1.0));
//        tf_listener.lookupTransform("odom", "base_link", ros::Time(0), stamped_transform);
//        double theta = tf::getYaw(stamped_transform.getRotation());
//        return theta;
//    }
//    catch (tf::TransformException &ex)
//    {
//        ROS_ERROR("1%s",ex.what());
//        ros::Duration(0.5).sleep();
//        return 0;
//
//    }
//
//}
//
//void Initial_Pose::rotate_circle()
//{
//  last_angle = get_odom_angle()/PI*180.0;//获取当前朝向，作为上一帧参考值
//  turn_angle = 0;//已经转过的角度归0；
//  double error = rotate_angle;//目标误差360度
//  ros::Rate loop_rate(20);
//  while (abs(error) > tolerance && ros::ok())
//  {
//    geometry_msgs::Twist rotate_msg;
//    rotate_msg.angular.z = 0.5;
//    cmd_pub.publish(rotate_msg);
//    loop_rate.sleep();
//    double odom_angle = get_odom_angle()/PI*180.0;//获取当前朝向
//    double delta_angle = normalize_angle(odom_angle - last_angle);//这段时间转了多少度
//    turn_angle += delta_angle;//累计转了多少度
//    ROS_INFO("turn_angle: %f\n",float(turn_angle));
//    error = rotate_angle - turn_angle;//还差多少度
//    last_angle = odom_angle;//这一帧朝向变为上一帧
//  }
//  cmd_pub.publish(geometry_msgs::Twist());//旋转完毕，停止了
//}
//
//void Initial_Pose::publish_initial_pos()
//{
//  geometry_msgs::PoseWithCovarianceStamped pose_msg;
//
//  pose_msg.header.stamp = ros::Time::now();
//  pose_msg.header.frame_id = "map";
//  pose_msg.pose.pose.position.x = x_pos;
//  pose_msg.pose.pose.position.y = y_pos;
//  pose_msg.pose.covariance[0] = 0.25;
//  pose_msg.pose.covariance[6 * 1 + 1] = 0.25;
//  pose_msg.pose.covariance[6 * 5 + 5] = 0.06853891945200942;
//  pose_msg.pose.pose.orientation.z = sin(alpha/2);
//  pose_msg.pose.pose.orientation.w = cos(alpha/2);
//  initial_pose_pub.publish(pose_msg);
//  ROS_INFO("Setting to :(%f,%f)",x_pos,y_pos);
//}
//
//double Initial_Pose::normalize_angle(double angle)
//{
//  double res = angle;
//  if (res>180.0)res-=360;
//  if (res<-180.0)res+=360;
//  return res;
//}

