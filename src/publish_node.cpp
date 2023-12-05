
#include "ros/ros.h"
#include "std_msgs/String.h" //use data struct of std_msgs/String  
#include "mbot_linux_serial.h"
#include "std_msgs/Float64MultiArray.h"
#include "chassis_package/ChassisMsg.h"

typedef chassis_package::ChassisMsg chassis_type;
using namespace std;

//test send value
double testSend1=5555.0;
double testSend2=2222.0;
unsigned char testSend3=0x07;

//test receive value
double testRece1=0.0;
double testRece2=0.0;
double testRece3=0.0;
double testRece4=0.0;
double testRece5=0.0;
unsigned char testRece6=0x00;

void printChassisData(const chassis_type& data){
    cout << "------------------------------------------" << endl;
    cout << "Raw RPM          : " << data.leftRawRPM << ", \t" << data.rightRawRPM << endl;
    cout << "Reas RPS         : " << data.leftRealRPS << ", \t" << data.rightRealRPS << endl;
    cout << "Angular vel(deg) : " << data.leftAngVelDeg << ", \t" << data.rightAngVelDeg << endl;
    cout << "Linear speed     : " << data.leftLinearSpeed << ", \t" << data.rightLinearSpeed << endl;
    cout << "Car forward speed: " << data.CarForwardSpeed << endl;
    cout << "Rotation ang vel : " << data.CarRotAngVelDeg << endl;
}


int main(int agrc,char **argv)
{
    ros::init(agrc,argv,"public_node");
    ROS_WARN("--> Chassis publisher start...");

    // ros::NodeHandle nh;
    std::shared_ptr<ros::NodeHandle> n = std::make_shared<ros::NodeHandle>("~");

    string port_name("/dev/ttyUSB0");
    string pub_topic("/chassis/data");
    int print_data(0);
    int pub_frequency(20);
    
    n->getParam("port", port_name);
    n->getParam("print_data", print_data);
    n->getParam("pub_topic", pub_topic);
    n->getParam("pub_frequency", pub_frequency);

    ROS_INFO("--------------------");
    ROS_INFO_STREAM("Open Port:  " << port_name);
    ROS_INFO_STREAM("Topic name: " << pub_topic);
    ROS_INFO_STREAM("Pub Rate(max 45): " << pub_frequency);
    ROS_INFO_STREAM("Print chassis?: " << print_data);
    ROS_INFO("--------------------");
    
    ros::Publisher pub_chassis = n->advertise<chassis_type>(pub_topic, 100);
    serialInit(port_name);

    chassis_type chassis;
    ros::Rate loop_rate(pub_frequency);
    int seq_counter = 0;
    while(ros::ok())
    {
        std_msgs::Float64MultiArray motor_msg;
        ros::spinOnce();
        // //向STM32端发送数据，前两个为double类型，最后一个为unsigned char类型
	    // writeSpeed(testSend1,testSend2,testSend3);
        //从STM32接收数据，输入参数依次转化为小车的线速度、角速度、航向角（角度）、预留控制位
        readSpeed(testRece1,testRece2,testRece3,testRece4,testRece5,testRece6);
        //打印数据
        // ROS_INFO("left: %f, \t right: %f ",testRece1,testRece2);

        double left_motor_rpm = testRece1;
        double right_motor_rpm = testRece2;

        // calculate other informations.
        const double reduction_ratio = 19.0f;          // M3508, 19:1
        const int number_of_section_per_cycle = 12;      // there are 12 section when the motor rotate one cycle
        const double length_of_per_section = 1.04e-2;    // each section is 1.04cm.
        const double car_width = 24e-2;                  // the width of the car is 24cm.

        chassis.leftRawRPM = left_motor_rpm;
        chassis.rightRawRPM = -right_motor_rpm;        // right motor is counterwise.
        chassis.leftRealRPS = chassis.leftRawRPM / reduction_ratio / 60;     // (rpm / 19 / 60)
        chassis.rightRealRPS = chassis.rightRawRPM / reduction_ratio / 60;

        chassis.leftAngVelDeg = chassis.leftRealRPS*360;
        chassis.rightAngVelDeg = chassis.rightRealRPS*360;
        chassis.leftLinearSpeed = chassis.leftRealRPS * number_of_section_per_cycle * length_of_per_section;
        chassis.rightLinearSpeed = chassis.rightRealRPS * number_of_section_per_cycle * length_of_per_section;
        chassis.CarForwardSpeed = (chassis.leftLinearSpeed + chassis.rightLinearSpeed) / 2;
        // rotation speed: left-rotation is position.
        chassis.CarRotAngVelDeg = (chassis.rightLinearSpeed - chassis.leftLinearSpeed) / (car_width / 2) * 180 / 3.141526;

        if(print_data)
            printChassisData(chassis);

        chassis.header.frame_id = "chassis";
        chassis.header.stamp = ros::Time::now();
        pub_chassis.publish(chassis);
        
        loop_rate.sleep();
    }
    return 0;
}

