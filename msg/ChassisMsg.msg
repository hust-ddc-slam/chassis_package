
# ChassisMsg

std_msgs/Header header      # ROS standard message header

float32 leftRawRPM          # left motor's rotation per minitues (raw data, before reduction)
float32 rightRawRPM
float32 leftRealRPS         # left motor's output rotation per second
float32 rightRealRPS
float32 leftAngVelDeg       # left motor's output angular velocity (in degree, after reduction)
float32 rightAngVelDeg

float32 leftLinearSpeed     # left linear speed (m/s)
float32 rightLinearSpeed

float32 CarForwardSpeed        # forward speed of the whole ChassisMsg
float32 CarRotAngVelDeg   # rotational angular velocity

# float32 leftAngVelRad
# float32 rightAngVelRad
