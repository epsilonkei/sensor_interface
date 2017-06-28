#include <takasago_sps/scpi_tcp_client.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "takasago_sps");
  ros::NodeHandle nh;
  ros::NodeHandle nh_private("~");

  TakasagoSps* TakasagoSpsNode = new TakasagoSps(nh, nh_private);

  ros::spin();
  delete TakasagoSpsNode;
  return 0;
}
