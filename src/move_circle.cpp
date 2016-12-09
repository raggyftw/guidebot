// Here we include the liberies and header files required
#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/Twist.h>
#include <stdlib.h>
// Declaring the function for moving to the starting point.
bool move_to_destination (double arr[2]);
// Declaring the function that makes the robot patrol
void patrol();
// Creating the variables that will store the location of starting point and patrol point
double start[2] = {0.0,0.0}; // Starting point
double circle_start[2] = {0.76,1.11}; //Enter coordinates of where to start patrol.
// Variable to check whether or not turtlebot has reached the plant
bool destination_reached = false;

// Declaring our contructor.
class patrol_node {
 // Declaring nodehandle and publisher for use in the constructor.
 private:
 ros::NodeHandle nh_;
 ros::Publisher cmd_patrol_pub;
public:
// Creating function that depends on our nodehandle.
 patrol_node(ros::NodeHandle &nh){
   nh_ = nh;
   // publishing our data of type geometry_msgs::Teist to the topic.
   cmd_patrol_pub = nh_.advertise <geometry_msgs::Twist>("cmd_vel_mux/input/teleop",1);
 }//End of patrol_node.

 void patrol ()
 {
   // We publish geometry_msgs as long as the node is running.
    while(nh_.ok()){
      // Declaring what type of data we publish
      geometry_msgs::Twist base_cmd;
      base_cmd.linear.x = base_cmd.linear.y = base_cmd.angular.z = 0;
      base_cmd.linear.x = 0.25;
      base_cmd.angular.z = 0.40;
      cmd_patrol_pub.publish(base_cmd);
    }//End of while loop.
  }//End of patrol.

};//End of patrol_node.

int main (int argc, char **argv)
{
// Here we initialize the ros node and name it.
ros::init(argc, argv, "find_plant_node");
// Creating the nodlehandle nh.
ros::NodeHandle nh;
// Moves the turtlebot to starting position.
//destination_reached = move_to_destination(circle_start);
patrol_node mover(nh); //Storing the patrol_node class information in mover.
// Running the patrol function.
mover.patrol();
}// End of main

// The function that moves the plant
bool move_to_destination(double arr[2])
{
  // Here a client is defined to send move goals to the move_base_server using a SimpleActionClient.
  actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true);
  // Displaying a message saying if the move_base action server is not running.
  while (!ac.waitForServer(ros::Duration(5.0))) {
    ROS_INFO ("Waiting for the move_base action server to come online or respond");
  }//End of while loop.
  // Declaring the move base variable starting_point.
  move_base_msgs::MoveBaseGoal starting_point;
  // Naming the map frame in which the turtlebot moves.
  starting_point.target_pose.header.frame_id = "map";
  // Setting the ros time stamp.
  starting_point.target_pose.header.stamp = ros::Time::now();
  // Telling the turtlebot where to go by moving it to the coordinates and facing forward.
  starting_point.target_pose.pose.position.x =  arr[0]; //This is the x coordinate of the desired location.
  starting_point.target_pose.pose.position.y =  arr[1]; //This is the y coordinate of the desired location.
  starting_point.target_pose.pose.position.z =  0.0;
  starting_point.target_pose.pose.orientation.x = 0.0;
  starting_point.target_pose.pose.orientation.y = 0.0;
  starting_point.target_pose.pose.orientation.z = 0.0;
  starting_point.target_pose.pose.orientation.w = 1.0; // This pose makes the turtlebot face front.
  // Sending message that the move goal is being sent.
  ROS_INFO("Sending move goal to move_base_server");
  // sending the move goal message.
  ac.sendGoal(starting_point);
  // Telling the program to not do anything before the actionclient responds with a result.
  ac.waitForResult();
  // checking if the turtlebots current position is equal to the plants location.
  if(ac.getState()== actionlib::SimpleClientGoalState::SUCCEEDED){
    return true;
  }//End of if statement.
  // If the turtlebots currelt position is not equal to the plants local display this.
  else {
    return false;
  }//End of else statement.
}//End of move_to_destination.
