cmake_minimum_required(VERSION 3.0.2)
project(chassis_package)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  std_msgs
  message_generation
)

add_message_files(FILES
  ChassisMsg.msg
)
generate_messages(DEPENDENCIES
  std_msgs
)

catkin_package(
  INCLUDE_DIRS include
  ${catkin_INCLUDE_DIRS}
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(chassis_node 
	src/publish_node.cpp 
	src/mbot_linux_serial.cpp)
target_link_libraries(chassis_node ${catkin_LIBRARIES})
