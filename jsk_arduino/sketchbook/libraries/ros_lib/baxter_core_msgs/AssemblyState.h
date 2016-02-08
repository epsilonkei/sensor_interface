#ifndef _ROS_baxter_core_msgs_AssemblyState_h
#define _ROS_baxter_core_msgs_AssemblyState_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace baxter_core_msgs
{

  class AssemblyState : public ros::Msg
  {
    public:
      bool enabled;
      bool stopped;
      bool error;
      uint8_t estop_button;
      uint8_t estop_source;
      enum { ESTOP_BUTTON_UNPRESSED =  0    };
      enum { ESTOP_BUTTON_PRESSED =  1 };
      enum { ESTOP_BUTTON_UNKNOWN =  2    };
      enum { ESTOP_BUTTON_RELEASED =  3    };
      enum { ESTOP_SOURCE_NONE =  0    };
      enum { ESTOP_SOURCE_USER =  1    };
      enum { ESTOP_SOURCE_UNKNOWN =  2    };
      enum { ESTOP_SOURCE_FAULT =  3    };
      enum { ESTOP_SOURCE_BRAIN =  4    };

    AssemblyState():
      enabled(0),
      stopped(0),
      error(0),
      estop_button(0),
      estop_source(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_enabled;
      u_enabled.real = this->enabled;
      *(outbuffer + offset + 0) = (u_enabled.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->enabled);
      union {
        bool real;
        uint8_t base;
      } u_stopped;
      u_stopped.real = this->stopped;
      *(outbuffer + offset + 0) = (u_stopped.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->stopped);
      union {
        bool real;
        uint8_t base;
      } u_error;
      u_error.real = this->error;
      *(outbuffer + offset + 0) = (u_error.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->error);
      *(outbuffer + offset + 0) = (this->estop_button >> (8 * 0)) & 0xFF;
      offset += sizeof(this->estop_button);
      *(outbuffer + offset + 0) = (this->estop_source >> (8 * 0)) & 0xFF;
      offset += sizeof(this->estop_source);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_enabled;
      u_enabled.base = 0;
      u_enabled.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->enabled = u_enabled.real;
      offset += sizeof(this->enabled);
      union {
        bool real;
        uint8_t base;
      } u_stopped;
      u_stopped.base = 0;
      u_stopped.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->stopped = u_stopped.real;
      offset += sizeof(this->stopped);
      union {
        bool real;
        uint8_t base;
      } u_error;
      u_error.base = 0;
      u_error.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->error = u_error.real;
      offset += sizeof(this->error);
      this->estop_button =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->estop_button);
      this->estop_source =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->estop_source);
     return offset;
    }

    const char * getType(){ return "baxter_core_msgs/AssemblyState"; };
    const char * getMD5(){ return "d7ff2b9fa7d5f688665ce44db4ee2af8"; };

  };

}
#endif