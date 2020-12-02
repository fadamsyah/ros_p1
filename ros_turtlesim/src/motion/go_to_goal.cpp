#include <math.h>
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include <iostream>

#define PI 3.14159  /* pi */

using namespace std;

ros::Publisher vel_pub;
ros::Subscriber pose_sub;
turtlesim::Pose turtlesim_pose;

void poseCallback(const turtlesim::Pose::ConstPtr& msg);
double constrainAngle(double x);
double getDistance(double x_goal, double y_goal);
double getDeltaYaw(double x_goal, double y_goal);
void go_to_goal(double x_goal, double y_goal, double tol_dst, double freq);


int main(int argc, char **argv)
{
    ros::init(argc, argv, "go_to_goal");
    ros::NodeHandle n;

    pose_sub = n.subscribe("/turtle1/pose", 10, poseCallback);
    vel_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);

    go_to_goal(1., 5., 0.05, 50.);

    return 0;
}

void poseCallback(const turtlesim::Pose::ConstPtr& msg){
    turtlesim_pose = *msg;
}

double constrainAngle(double x){
    x = fmod(x + 180, 360);
    if (x < 0)
        x += 360;
    return x - 180;
}

double getDistance(double x_goal, double y_goal){
    double dx = x_goal - turtlesim_pose.x;
    double dy = y_goal - turtlesim_pose.y;
    return sqrt(pow(dx,2) + pow(dy,2));
}

double getDeltaYaw(double x_goal, double y_goal){
    double dx = x_goal - turtlesim_pose.x;
    double dy = y_goal - turtlesim_pose.y;
    return constrainAngle((atan2(dy,dx) - turtlesim_pose.theta) * 180. / PI) * PI / 180.;
}

void go_to_goal(double x_goal, double y_goal, double tol_dst, double freq){
    double dst, dyaw;
    geometry_msgs::Twist vel_msg;
    ros::Rate loop_rate(freq);

    double k1 = 3.;
    double k2 = 3.;
    double k3 = 5.;
    
    do{
        dst = getDistance(x_goal, y_goal);
        dyaw = getDeltaYaw(x_goal, y_goal);

        vel_msg.linear.x = k1 * (1 - exp(-k2*dst));
        vel_msg.angular.z = k3 * dyaw;
        
        vel_pub.publish(vel_msg);
        ros::spinOnce();
        loop_rate.sleep();

    } while(dst > tol_dst);

    vel_msg.linear.x = 0.;
    vel_msg.angular.z = 0.;
    vel_pub.publish(vel_msg);
}