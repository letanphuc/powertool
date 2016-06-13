/*
 * raspi_ext.h
 *
 *  Created on: Oct 7, 2015
 *      Author: kieutq
 */

#ifndef SRC_RASPI_EXT_H_
#define SRC_RASPI_EXT_H_

//#include "global_config.h"

//typedef enum LED_Mode
//{
//	LED_On = 1,
//	LED_Off,
//	LED_Toggle
//} LED_Mode_t;
#define LED_MODE_ON			0x01
#define LED_MODE_OFF		0x02
#define LED_MODE_TOGGLE		0x03

typedef struct Time
{
  unsigned char hour;
  unsigned char minute;
  unsigned char second;
  unsigned char day_of_week;
} Time_t;

typedef struct Date
{
	unsigned char day;
	unsigned char month;
	unsigned char year;
	unsigned char control;
} Date_t;


extern int RaspiExt_Init(void);
extern int RaspiExt_PowerOff(void);
extern int RaspiExt_DestroyAll(void);
extern int RaspiExt_WaitForExit(void);
extern int RaspiExt_LED_Run_Config(unsigned char mode, int time_ms);
extern int RaspiExt_LED_Power_Config(unsigned char mode, int time_ms);
extern int RaspiExt_LED_Hostx_Config(unsigned char mode, int time_ms, int host);
extern int RaspiExt_Pin_Hostx_Active(int host);
extern int RaspiExt_Pin_Hostx_Inactive(int host);
extern int RaspiExt_DS1307_GetTime(Time_t * t);
extern int RaspiExt_DS1307_SetTime(Time_t t);
extern int RaspiExt_DS1307_GetDate(Date_t * d);
extern int RaspiExt_DS1307_SetDate(Date_t d);

#endif /* SRC_RASPI_EXT_H_ */
