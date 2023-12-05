#ifndef MBOT_LINUX_SERIAL_H
#define MBOT_LINUX_SERIAL_H

#include <ros/ros.h>
#include <ros/time.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <boost/asio.hpp>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <string>
#define send_data_num 2

extern void serialInit(std::string port_name);
extern void writeSpeed(double Left_v, double Right_v,unsigned char ctrlFlag);
extern bool readSpeed(double &Left_v,double &Right_v,double &Angle,double &pos_x,double &pos_y, unsigned char &ctrlFlag);
unsigned char getCrc8(unsigned char *ptr, unsigned short len);

#endif
