# chassis_package
This package read the chassis data and publish.

## Usage
```bash
# sudo chmod 777 /dev/ttyUSB0
roslaunch chassis_package chassis.launch
```

**Configures:**  
- `port`: USB port name.
- `pub_topic`: output chassis data topic name.
- `print_data`: print chassis data or not?
- `pub_frequency`: the chassis publish rate.

## Details
The code first get data from the stm32f4, and then calculate the `ChassisMsg` data, which includes all chassis data.  

## Issues
`leftRawRPM`/`rightRawRPM` is from feedback, which is correct.  
Others are calcualted, may not be accurate. Calibration is needed.  

