# Declare the version of the CMake API for forward-compatibility
cmake_minimum_required(VERSION 2.8)

# Declare the name of the CMake Project
project(guidebot)



# Find Catkin
find_package(catkin REQUIRED
  roscpp
  rospy
  std_msgs
  tf
  geometry_msgs
  move_base_msgs
  actionlib
  sensor_msgs 
  cv_bridge
  image_transport
  )
find_package(Boost REQUIRED)
# Declare this project as a catkin package
catkin_package()

# Add the local headers and the headers from roscpp
include_directories(include	
	${catkin_INCLUDE_DIRS}		    
)


# Define the executables used in guidebot.
add_executable(find_plant3 src/find_plant3.cpp)
add_executable(move_circle src/move_circle.cpp)
add_executable(view_depth src/view_depth.cpp)
 
# link libraries of the target files. 
target_link_libraries(find_plant3 ${catkin_LIBRARIES})
target_link_libraries(move_circle ${catkin_LIBRARIES}) 
target_link_libraries(view_depth ${catkin_LIBRARIES})
