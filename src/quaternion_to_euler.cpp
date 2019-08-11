#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>

class test_node
{
private:
  ros::Subscriber sub_odom;
  nav_msgs::Odometry odom;
  tf::TransformBroadcaster odom_broadcaster;

  void cb_odom(const nav_msgs::Odometry::ConstPtr &msg);

public:
  test_node();
  void mainloop();
  void tf_quat_to_rpy(double& roll, double& picth, double& yaw, geometry_msgs::Quaternion q);
};

void test_node::cb_odom(const nav_msgs::Odometry::ConstPtr &msg)
{
  odom = *msg;
}

test_node::test_node()
{
  ros::NodeHandle nh;
  sub_odom = nh.subscribe("/odom", 5, &test_node::cb_odom, this);
}

void test_node::tf_quat_to_rpy(double& roll, double& pitch, double& yaw, geometry_msgs::Quaternion q)
{
  tf::Quaternion quat;
  quaternionMsgToTF(q, quat);
  tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);
}

void test_node::mainloop()
{
  ros::Time current_time;
  current_time = ros::Time::now();
  ros::Rate rate(10.0);

  double euler_x = 0.0;
  double euler_y = 0.0;
  double euler_z = 0.0;

  while (ros::ok())
  {
    ros::spinOnce();
    
    test_node::tf_quat_to_rpy(euler_x, euler_y, euler_z, odom.pose.pose.orientation);

    ROS_INFO("euler_z %f", euler_z);
  }
}

int main (int argc, char** argv)
{
  ros::init(argc, argv, "trans_euler");
  test_node robot_test;
  robot_test.mainloop();
  
  return 0;
}
