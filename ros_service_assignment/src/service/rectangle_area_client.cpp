#include "ros/ros.h"
#include "ros_service_assignment/RectangleArea.h"
#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char **argv){

    ros::init(argc, argv, "rectangle_area_client");
    if (argc != 3)
    {
        ROS_INFO("usage: rectangle_area_client length width");
        return 1;
    }

    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<ros_service_assignment::RectangleArea>("rectangle_area");

    ros_service_assignment::RectangleArea srv;
    srv.request.l = atof(argv[1]);
    srv.request.w = atof(argv[2]);
    if (client.call(srv)){
        cout << "Area = " << (double)srv.response.area << endl;
    }
    else{
        ROS_ERROR("Failed to call service rectangle_area");
        return 1;
    }

    return 0;
}