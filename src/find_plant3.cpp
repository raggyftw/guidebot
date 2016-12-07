// Here we include the liberies and header files required
#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <stdlib.h>
// Declaring the function for moving to the plant.
bool move_to_destination (double arr[2]);
// Creating the variables that will store the location of the plant.
double plant[2] = {1.03,4.12};
double start[2] = {0.0,0.0};
// Variable to check whether or not turtlebot has reached the plant.
bool destination_reached = false;
// Declaring variable for user input.
int user_input;

int main (int argc, char **argv)
{
// Here we initialize the ros node and name it.
ros::init(argc, argv, "find_plant_node");
// Creating the nodlehandle nh.
ros::NodeHandle nh;
std::cout<<"Press 1 to move to the plant, press 0 to return to start"<<std::endl;
std::cin>>user_input;
// If user presses one the turtlebot moves to plant and then back to start.
if (user_input == 1){
  destination_reached = move_to_destination(plant);
  destination_reached = move_to_destination(start);
  std::cout<<"Moving the the plant"<<std::endl;
}//End of if statement.
// If user presses 0 the turtlebot moves back to start.
else if (user_input ==0){
  destination_reached = move_to_destination(start);
  std::cout<<"Moving back to start"<<std::endl;
}//End of else if statement.
return 0;
}//End of main.

// The function that moves the plant.
bool move_to_destination(double arr[2])
{
  // Here a client is defined to send move goals to the move_base_server using a SimpleActionClient.
  actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true); //The server is named move_base.
  // Displaying a message saying if the move_base action server is not running.
  while (!ac.waitForServer(ros::Duration(5.0))) { // Waits for 5 seconds.
    ROS_INFO ("Waiting for the move_base action server to come online or respond");
  }//End of while loop
  // Declaring the move base variable goal.
    move_base_msgs::MoveBaseGoal goal;
  // Naming the map frame in which the turtlebot moves.
  goal.target_pose.header.frame_id = "map";
  // Setting the ros time stamp.
  goal.target_pose.header.stamp = ros::Time::now();
  // Telling the turtlebot where to go by moving it to the coordinates and facing forward..
  goal.target_pose.pose.position.x =  arr[0]; //This is the x coordinate of the desired location.
  goal.target_pose.pose.position.y =  arr[1]; //This is the y coordinate of the desired location.
  goal.target_pose.pose.position.z =  0.0;
  goal.target_pose.pose.orientation.x = 0.0;
  goal.target_pose.pose.orientation.y = 0.0;
  goal.target_pose.pose.orientation.z = 0.0;
  goal.target_pose.pose.orientation.w = 1.0; // This pose makes the turtlebot face front.
  // Sending message that the move goal is being sent.
  ROS_INFO("Sending move goal to move_base_server");
  // sending the move goal message.
  ac.sendGoal(goal);
  // Telling the program to not do anything before the actionclient responds with a result.
  ac.waitForResult();
  // checking if the turtlebots current position is equal to the plants location.
  if(ac.getState()== actionlib::SimpleClientGoalState::SUCCEEDED){
    ROS_INFO("Turtlebot has reached the goal");
    return true;
  }//End of if statement.
  // If the turtlebots currelt position is not equal to the plants local display this.
  else {
    ROS_INFO("The turtlebot failed to reach the goal");
    return false;
  }//End of else statement.
}//End of move_to_destination.
