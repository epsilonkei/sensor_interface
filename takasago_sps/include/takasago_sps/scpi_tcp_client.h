#include <ros/ros.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <takasago_sps/PowerInfo.h>
#include <std_msgs/Empty.h>

class TakasagoSps
{
public:
  TakasagoSps(ros::NodeHandle nh, ros::NodeHandle nhp);
  ~TakasagoSps(){   close(sd_);}

private:
  ros::NodeHandle nh_;
  ros::NodeHandle nhp_;
  ros::Publisher  power_info_pub_;
  ros::Subscriber  power_on_sub_, power_off_sub_;

  bool power_flag_;
  ros::Timer timer_;
  double tm_loop_rate_;
  double voltage_;

  //socket
  int sd_;
  char read_buf_[200];
  struct sockaddr_in addr_;
  std::string ip_address_;
  int port_;


  void powerOnCallback(const std_msgs::EmptyConstPtr &msg);
  void powerOffCallback(const std_msgs::EmptyConstPtr &msg);
  void powerInfoFunction(const ros::TimerEvent & e);

};
