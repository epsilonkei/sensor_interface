/* =============================================================================
  LIDAR-Lite v2: Single sensor get single distance measurement

  This example file demonstrates how to take a single distance measurement with
  LIDAR-Lite v2 "Blue Label".

  The library is in BETA, so subscribe to the github repo to recieve updates, or
  just check in periodically:
  https://github.com/PulsedLight3D/LIDARLite_v2_Arduino_Library
  
  To learn more read over lidarlite.cpp as each function is commented
=========================================================================== */

#include <ros.h>
#include <std_msgs/Int16.h>
#include <Wire.h>
#include <LIDARLite.h>


LIDARLite myLidarLite;
ros::NodeHandle  nh;
std_msgs::Int16 lidarDist;
ros::Publisher pubDist("lidar_lite/distance", &lidarDist);

void setup() {
  nh.getHardware()->setBaud(115200);
  nh.initNode();

  nh.advertise(pubDist);
  //Serial.begin(115200);
  myLidarLite.begin(1, true);
  //myLidarLite.begin(0, true);
}

void loop() {
  lidarDist.data = myLidarLite.distance(false, false);
  //lidarDist.data = myLidarLite.distance(true, false);
  if(lidarDist.data < 400)
    pubDist.publish(&lidarDist);
  nh.spinOnce();
  delay(10);
  //Serial.println(myLidarLite.distance());
}