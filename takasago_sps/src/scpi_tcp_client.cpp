#include <takasago_sps/scpi_tcp_client.h>

#include <sys/ioctl.h>

TakasagoSps::TakasagoSps(ros::NodeHandle nh, ros::NodeHandle nhp): nh_(nh), nhp_(nhp)
{
  nhp_.param("tm_loop_rate", tm_loop_rate_, 10.0);
  nhp_.param("port", port_, 50001);
  nhp_.param("ip_address", ip_address_, std::string("192.168.0.1"));

  //socket

  if((sd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    ROS_ERROR("can not open socket");
    ros::shutdown();
    return;
  }

  // int nonblocking = 1;
  // ioctl(sd_, FIONBIO, &nonblocking);

  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(port_);
  addr_.sin_addr.s_addr = inet_addr(ip_address_.c_str());
  connect(sd_, (struct sockaddr *)&addr_, sizeof(struct sockaddr_in));

  //sleep(1); //tmp
  //initialize
  if  (write(sd_, "ADDR 1\n", 7)==-1 )
    {
      ROS_ERROR("can not send socket");
      ros::shutdown();
      return;
    }
  memset(read_buf_, 0, sizeof(read_buf_));
  while (read(sd_, read_buf_, sizeof(read_buf_)) <= 0);
  printf("%s", read_buf_);
  if(!strstr(read_buf_, "OK"))
    {
      ROS_ERROR("can not initialize");
      ros::shutdown();
      return;
    }

  if  (write(sd_, "ALM:CLE\n", 8)==-1 )
    {
      ROS_ERROR("can not send socket");
      ros::shutdown();
      return;
    }
  memset(read_buf_, 0, sizeof(read_buf_));
  while (read(sd_, read_buf_, sizeof(read_buf_)) <= 0);
  printf("%s", read_buf_);

  if  (write(sd_, "*IDN?\n",6)==-1 )
    {
      ROS_ERROR("can not send socket");
      ros::shutdown();
      return;
    }
  memset(read_buf_, 0, sizeof(read_buf_));
  //sleep(1); // tmp
  while (read(sd_, read_buf_, sizeof(read_buf_)) <= 0);
  ROS_INFO("initialized: %s", read_buf_);


  power_info_pub_ = nh_.advertise<takasago_sps::PowerInfo>("power_info", 5);
  power_on_sub_ = nh_.subscribe<std_msgs::Empty>("power_on_cmd", 1, &TakasagoSps::powerOnCallback, this, ros::TransportHints().tcpNoDelay());
  power_off_sub_ = nh_.subscribe<std_msgs::Empty>("power_off_cmd", 1, &TakasagoSps::powerOffCallback, this, ros::TransportHints().tcpNoDelay());
  timer_ = nhp_.createTimer(ros::Duration(1.0 / tm_loop_rate_), &TakasagoSps::powerInfoFunction,this);

  //while (read(sd_, read_buf_, sizeof(read_buf_)) > 0);
}


void TakasagoSps::powerOnCallback(const std_msgs::EmptyConstPtr &msg)
{
  if  (write(sd_, "OUTP ON\n" ,8)==-1 )
    {
      ROS_ERROR("can not send socket");
      return;
    }

  memset(read_buf_, 0, sizeof(read_buf_));
  read(sd_, read_buf_, sizeof(read_buf_));

  //bad
  ROS_ERROR("power on!");
  power_flag_ = true;

  /*
  if(strstr(read_buf_, "OK"))
    {
      ROS_ERROR("power on!");
      power_flag_ = true;
    }
  else
    {
      ROS_ERROR("%s", read_buf_);
    }
  */
}

void TakasagoSps::powerOffCallback(const std_msgs::EmptyConstPtr &msg)
{
  if  (write(sd_, "OUTP OFF\n" ,9)==-1 )
    {
      ROS_ERROR("can not send socket");
      return;
    }

  memset(read_buf_, 0, sizeof(read_buf_));
  read(sd_, read_buf_, sizeof(read_buf_));

  //bad
  ROS_ERROR("power off!");
  power_flag_ = false;

  /*
  if(strstr(read_buf_, "OK"))
    {
      ROS_ERROR("power off!");
      power_flag_ = false;
    }
  else
    {
      ROS_ERROR("%s", read_buf_);
    }
  */

}

void TakasagoSps::powerInfoFunction(const ros::TimerEvent & e)
{
  // if(!power_flag_) return;
  // while (read(sd_, read_buf_, sizeof(read_buf_)) > 0)
  //   {
  //     ROS_ERROR("message %s", read_buf_);
  //   }

  takasago_sps::PowerInfo power_info;
  std::stringstream ss;

  if  (write(sd_, "MEAS:CURR?\n",11)==-1 )
    {
      ROS_ERROR("can not send socket");
      return;
    }
  memset(read_buf_, 0, sizeof(read_buf_));
  //usleep(50000);
  while (read(sd_, read_buf_, sizeof(read_buf_)) <= 0);

  ss.str(std::string(read_buf_));
  ss >>power_info.current;

  if  (write(sd_, "MEAS:VOLT?\n",11)==-1 )
    {
      ROS_ERROR("can not send socket");
      return;
    }
  memset(read_buf_, 0, sizeof(read_buf_));
  //usleep(50000);
  while (read(sd_, read_buf_, sizeof(read_buf_)) <= 0);

  ss.str(std::string(read_buf_));
  ss >>power_info.voltage;

  if  (write(sd_, "MEAS:POW?\n",11)==-1 )
    {
      ROS_ERROR("can not send socket");
      return;
    }
  memset(read_buf_, 0, sizeof(read_buf_));
  while (read(sd_, read_buf_, sizeof(read_buf_)) <= 0);

  ss.str(std::string(read_buf_));
  ss >>power_info.power;

  //ROS_INFO("%f", power_info.current);
  power_info_pub_.publish(power_info);

}
