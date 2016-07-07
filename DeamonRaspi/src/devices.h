/*
 * sensor.h
 *
 *  Created on: Sep 17, 2015
 *      Author: kieu
 */

#ifndef DEVICES_H_
#define DEVICES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * this file contain devices communication source code
 * used for get or set data to devices
 */

//#include "global_config.h"
#include "packet.h"
#include <pthread.h>


#ifndef DEVICE_DEBUG
#define DEVICE_DEBUG 1
#endif

#ifndef DEVICE_DATABASE
#define DEVICE_DATABASE 0
#endif


struct polling_control
{
	unsigned char destroy;
	unsigned char enable;
	unsigned int time_poll_ms;
};

struct Device
{
	//
	struct polling_control polling_control;

    unsigned long last_try;
	// required packet.h
	struct PacketData dev_data;
    float value;
};


union int_s
{
  uint32_t n;
  uint8_t b[4];
};
union float_s
{
	float f;
	uint8_t b[4];
};


#define DEV_HOST_NUMBER 4 // 4 USB interfaces

extern struct Device dev_host[DEV_HOST_NUMBER];

extern int Device_Init(void);
extern int Device_Polling(void);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_H_ */
