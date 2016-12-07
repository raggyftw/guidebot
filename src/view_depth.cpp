#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
// Here the publising queue for the subscriber is set to 1000.
static const uint32_t ROS_QUEUE = 1000;
// Declaring the variables need in the function.
float dist_val = 0; //Float variable to store the depth data in meters.
bool kill = false; // Bool to check if rosnode kill has been performed.
// Creating our callback function.
void Depth_callback(const sensor_msgs::Image::ConstPtr& msg)
{
    try {
        // Declaring cv pointer and filling it with msg information.
        cv_bridge::CvImageConstPtr cv_ptr;
        cv_ptr = cv_bridge::toCvShare(msg);

        // Setting the global maximum for the data type.
        double max = 0.0;
        // Returning max value of the array.
        cv::minMaxLoc(cv_ptr->image, 0, &max, 0, 0);
        // Declaring the Mat matrix.
        cv::Mat normalized;
        // Here the mat matrix is normalized and converted so we can use the data in the matrix.
        cv_ptr->image.convertTo(normalized, CV_32F, 1.0/max, 0);


        // Declaring counter.
        int counter = 0;
        // using 2 for loops to make a line scan of the depth/Image.
        for (int i = 239; i <= 240; i++){
          for(int j = 209; j <= 429; j++){
              // stores the scan data in dist_val
              dist_val = cv_ptr->image.at<float>( i,j );
                // If statement to check when object is in front of turtbot.
                if (dist_val < 1.1){
                  counter++;
                  if(counter== 10 && kill == false){
                  kill = true;
                  // Killing the currently running nodes.
                  system("rosnode kill move_circle");
                  system("rosnode kill view_depth");
                  // Launches the next node.
                  system("roslaunch modular_lib_pkg move_plant.launch");
                  }//End of if statment.
                }//End of if statement.
          }//Ending for loop.
        }//Ending for loop.




    }//End of try .
    // Here any exception are handled.
     catch (const cv_bridge::Exception& e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
    }//End of catch.
}//End of Depth_callback.

int main(int argc, char* argv[])
{
    // initializing ros and naming the node.
    ros::init(argc, argv, "view_depth");
    // Creating nodehandler.
    ros::NodeHandle nh;
    // Subscribing to the depth image and returning the callback.
    ros::Subscriber sub = nh.subscribe("camera/depth/image", ROS_QUEUE, Depth_callback);
    ros::spin();
    return 0;
}//End of main.
