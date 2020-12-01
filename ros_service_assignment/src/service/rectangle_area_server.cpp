#include "ros/ros.h"
#include "ros_service_assignment/RectangleArea.h"
#include <iostream>

using namespace std;

bool rectangle_area(ros_service_assignment::RectangleArea::Request &req,
                    ros_service_assignment::RectangleArea::Response &res)
{
    res.area = req.l * req.w;
    cout << "Request: length = " << req.l << ", width = " << req.w << endl;
    cout << "Sending back response: area = " << res.area;
    return true;
}

int main(int argc, char **argv){

    ros::init(argc, argv, "rectangle_area_server");
    ros::NodeHandle n;

    ros::ServiceServer service = n.advertiseService("rectangle_area", rectangle_area);
    ROS_INFO("Ready to calculate an area of rectangle.");
    ros::spin();

    return 0;
}