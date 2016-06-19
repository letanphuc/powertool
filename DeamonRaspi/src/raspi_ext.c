/*
 * raspi_ext.c
 *
 *  Created on: Oct 7, 2015
 *      Author: kieutq
 */

#include "raspi_ext.h"
#include "../wiringPi/wiringPi.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "devices.h"

// global control parameter for threading
struct LED_Control
{
	unsigned char mode;
	unsigned char state;
	unsigned int time_ms;
	void (*On) (unsigned char * currState);
	void (*Off) (unsigned char * currState);
	void (*Toggle) (unsigned char * currState);
};

struct LED_Control LED_Run;
struct LED_Control LED_Power;
struct LED_Control LED_Host1;
struct LED_Control LED_Host2;
struct LED_Control LED_Host3;
struct LED_Control LED_Host4;

pthread_t led_run_thread;
pthread_t led_power_thread;
pthread_t led_host1_thread;
pthread_t led_host2_thread;
pthread_t led_host3_thread;
pthread_t led_host4_thread;
pthread_t power_off_button_thread;

pthread_mutex_t led_token = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t button_token;

#define LED_RUN_PIN		24
#define LED_POWER_PIN	25
#define LED_HOST1_PIN	7
#define LED_HOST2_PIN	0
#define LED_HOST3_PIN	2
#define LED_HOST4_PIN	3
#define POWER_PIN		4 // to cutoff power
#define POWER_OFF_PIN	1

#define HOST_SE1		5
#define HOST_SE2		6
#define HOST_SE3		21
#define HOST_SE4		22

void LED_Run_On(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Turn on led run.\n");
#endif
	digitalWrite(LED_RUN_PIN, HIGH);
	*currState = 1;
}
void LED_Run_Off(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Turn off led run.\n");
#endif
	digitalWrite(LED_RUN_PIN, LOW);
	*currState = 0;
}
void LED_Run_Toggle(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Toggle led run.\n");
#endif
	if (*currState)
	{
		digitalWrite(LED_RUN_PIN, LOW);
		*currState = 0;
	}
	else
	{
		digitalWrite(LED_RUN_PIN, HIGH);
		*currState = 1;
	}
}
void LED_Power_On(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Turn on led power.\n");
#endif
	digitalWrite(LED_POWER_PIN, HIGH);
	*currState = 1;
}
void LED_Power_Off(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Turn off led power.\n");
#endif
	digitalWrite(LED_POWER_PIN, LOW);
	*currState = 0;
}
void LED_Power_Toggle(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Toggle led power.\n");
#endif
	if (*currState)
	{
		digitalWrite(LED_POWER_PIN, LOW);
		*currState = 0;
	}
	else
	{
		digitalWrite(LED_POWER_PIN, HIGH);
		*currState = 1;
	}
}
void LED_Host1_On(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Turn on led host 1.\n");
#endif
	digitalWrite(LED_HOST1_PIN, HIGH);
	*currState = 1;
}
void LED_Host1_Off(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Turn off led host 1.\n");
#endif
	digitalWrite(LED_HOST1_PIN, LOW);
	*currState = 0;
}
void LED_Host1_Toggle(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Toggle led host 1.\n");
#endif
	if (*currState)
	{
		digitalWrite(LED_HOST1_PIN, LOW);
		*currState = 0;
	}
	else
	{
		digitalWrite(LED_HOST1_PIN, HIGH);
		*currState = 1;
	}
}
void LED_Host2_On(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Turn on led host 2.\n");
#endif
	digitalWrite(LED_HOST2_PIN, HIGH);
	*currState = 1;
}
void LED_Host2_Off(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Turn off led host 2.\n");
#endif
	digitalWrite(LED_HOST2_PIN, LOW);
	*currState = 0;
}
void LED_Host2_Toggle(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Toggle led host 2.\n");
#endif
	if (*currState)
	{
		digitalWrite(LED_HOST2_PIN, LOW);
		*currState = 0;
	}
	else
	{
		digitalWrite(LED_HOST2_PIN, HIGH);
		*currState = 1;
	}
}
void LED_Host3_On(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Turn on led host 3.\n");
#endif
	digitalWrite(LED_HOST3_PIN, HIGH);
	*currState = 1;
}
void LED_Host3_Off(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Turn off led host 3.\n");
#endif
	digitalWrite(LED_HOST3_PIN, LOW);
	*currState = 0;
}
void LED_Host3_Toggle(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Toggle led host 3.\n");
#endif
	if (*currState)
	{
		digitalWrite(LED_HOST3_PIN, LOW);
		*currState = 0;
	}
	else
	{
		digitalWrite(LED_HOST3_PIN, HIGH);
		*currState = 1;
	}
}
void LED_Host4_On(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Turn on led host 4.\n");
#endif
	digitalWrite(LED_HOST4_PIN, HIGH);
	*currState = 1;
}
void LED_Host4_Off(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Turn off led host 4.\n");
#endif
	digitalWrite(LED_HOST4_PIN, LOW);
	*currState = 0;
}
void LED_Host4_Toggle(unsigned char * currState)
{
#if RASPI_EXT_DEBUG
	printf("Toggle led host 4.\n");
#endif
	if (*currState)
	{
		digitalWrite(LED_HOST4_PIN, LOW);
		*currState = 0;
	}
	else
	{
		digitalWrite(LED_HOST4_PIN, HIGH);
		*currState = 1;
	}
}

void * LED_Thread(void * parameters)
{
	struct LED_Control * led_ctrl = (struct LED_Control *) parameters;
	unsigned int toggle_time = 0;

	for (;;)
	{
		if (pthread_mutex_trylock(&led_token) == 0)
		{
			switch (led_ctrl->mode)
			{
			case LED_MODE_ON:
				if (led_ctrl->time_ms > 0)
				{
					if (led_ctrl->state == 0)
						led_ctrl->On(&(led_ctrl->state));
					led_ctrl->time_ms--;
				}
				else if (led_ctrl->time_ms == 0)
				{
					if (led_ctrl->state == 1)
						led_ctrl->Off(&(led_ctrl->state));
				}
				else
				{
					if (led_ctrl->state == 0)
						led_ctrl->On(&(led_ctrl->state));
				}
				break;
			case LED_MODE_OFF:
				if (led_ctrl->state == 1)
					led_ctrl->Off(&(led_ctrl->state));
				break;
			case LED_MODE_TOGGLE:
				if (toggle_time == 0)
				{
					led_ctrl->Toggle(&(led_ctrl->state));
					toggle_time = led_ctrl->time_ms;
				}
				if (toggle_time > 0)
				{
					toggle_time--;
				}
				break;
			default:
			#if RASPI_EXT_DEBUG
				printf("LED Thread: Invalid LED mode %d.\n", led_ctrl->mode);
				#endif
				break;
			}
		
			pthread_mutex_unlock(&led_token);
		}
		else
		{
#if RASPI_EXT_DEBUG
			printf("LED Thread: Cannot access led token.\n", led_ctrl->mode);
#endif
		}
		usleep(1000);
	}
	return 0;
}

void * ButtonThread (void * params)
{
	int hold_time = 0;

	for (;;)
	{
		//pthread_mutex_lock(&button_token);
		if (digitalRead(POWER_OFF_PIN) == LOW)
		{
			hold_time ++;
		}
		else
		{
			hold_time = 0;
		}
		if (hold_time > 3) // about 1.5 seconds
		{
#if RASPI_EXT_DEBUG
			printf("Power off detected.\n");
			printf("Shutdown system.\n");
#endif
			RaspiExt_PowerOff();
		}
		//pthread_mutex_unlock(&button_token);
		usleep(500000);
	}
	return 0;
}

int RaspiExt_Init(void)
{
#if !SIMULATE_DATA
	if (wiringPiSetup() != 0)
	{
#if RASPI_EXT_DEBUG
		printf("Initial wiringPi fail.\n");
#endif
		return -1;
	}
#endif
#if RASPI_EXT_DEBUG
	printf("Initial wiringPi successful.\n");
#endif

	pinMode(LED_RUN_PIN, OUTPUT);
	pinMode(LED_POWER_PIN, OUTPUT);
	pinMode(LED_HOST1_PIN, OUTPUT);
	pinMode(LED_HOST2_PIN, OUTPUT);
	pinMode(LED_HOST3_PIN, OUTPUT);
	pinMode(LED_HOST4_PIN, OUTPUT);
	pinMode(POWER_PIN, OUTPUT);
	pinMode(POWER_OFF_PIN, INPUT);
	pinMode(HOST_SE1, OUTPUT);
	pinMode(HOST_SE2, OUTPUT);
	pinMode(HOST_SE3, OUTPUT);
	pinMode(HOST_SE4, OUTPUT);

	// hold on power pin
	digitalWrite(POWER_PIN, HIGH);

	// deactive all select pin
	digitalWrite(HOST_SE1, HIGH);
	digitalWrite(HOST_SE2, HIGH);
	digitalWrite(HOST_SE3, HIGH);
	digitalWrite(HOST_SE4, HIGH);


	LED_Run.mode = LED_MODE_TOGGLE;
	LED_Run.state = 0;
	LED_Run.time_ms = 100;
	LED_Run.On = LED_Run_On;
	LED_Run.Off = LED_Run_Off;
	LED_Run.Toggle = LED_Run_Toggle;

	LED_Power.mode = LED_MODE_ON;
	LED_Power.state = 0;
	LED_Power.time_ms = -1;
	LED_Power.On = LED_Power_On;
	LED_Power.Off = LED_Power_Off;
	LED_Power.Toggle = LED_Power_Toggle;

	LED_Host1.mode = LED_MODE_TOGGLE;
	LED_Host1.state = 0;
	LED_Host1.time_ms = 100;
	LED_Host1.On = LED_Host1_On;
	LED_Host1.Off = LED_Host1_Off;
	LED_Host1.Toggle = LED_Host1_Toggle;

	LED_Host2.mode = LED_MODE_TOGGLE;
	LED_Host2.state = 0;
	LED_Host2.time_ms = 100;
	LED_Host2.On = LED_Host2_On;
	LED_Host2.Off = LED_Host2_Off;
	LED_Host2.Toggle = LED_Host2_Toggle;

	LED_Host3.mode = LED_MODE_TOGGLE;
	LED_Host3.state = 0;
	LED_Host3.time_ms = 100;
	LED_Host3.On = LED_Host3_On;
	LED_Host3.Off = LED_Host3_Off;
	LED_Host3.Toggle = LED_Host3_Toggle;

	LED_Host4.mode = LED_MODE_TOGGLE;
	LED_Host4.state = 0;
	LED_Host4.time_ms = 100;
	LED_Host4.On = LED_Host4_On;
	LED_Host4.Off = LED_Host4_Off;
	LED_Host4.Toggle = LED_Host4_Toggle;
		
	pthread_mutex_init(&led_token, NULL);
	//pthread_mutex_unlock(&led_token);
	//pthread_mutex_init(&button_token, NULL);
#if !SIMULATE_DATA
#if RASPI_EXT_DEBUG
	printf("Create thread for control LED.\n");
#endif
	pthread_create(&led_run_thread, NULL, &LED_Thread, (void *)&LED_Run);
	pthread_create(&led_power_thread, NULL, &LED_Thread, (void *)&LED_Power);
	pthread_create(&led_host1_thread, NULL, &LED_Thread, (void *)&LED_Host1);
	pthread_create(&led_host2_thread, NULL, &LED_Thread, (void *)&LED_Host2);
	pthread_create(&led_host3_thread, NULL, &LED_Thread, (void *)&LED_Host3);
	pthread_create(&led_host4_thread, NULL, &LED_Thread, (void *)&LED_Host4);

#if RASPI_EXT_DEBUG
	printf("Create thread for control power button.\n");
#endif
	pthread_create(&power_off_button_thread, NULL, &ButtonThread, NULL);
#endif

	return 0;
}


int RaspiExt_PowerOff(void)
{
	// hold down power pin to cutoff directly power
	//digitalWrite(POWER_PIN, LOW);
	// or just call system shutdown command
	system("shutdown -h now");

	return 0;
}

int RaspiExt_LED_Run_Config(unsigned char mode, int time_ms)
{
	if (mode != LED_MODE_ON && mode != LED_MODE_OFF && mode != LED_MODE_TOGGLE)
	{
#if RASPI_EXT_DEBUG
		printf("LED Run Config: Not own mode.\n");
#endif
		return -1;
	}
	pthread_mutex_lock(&led_token);
	LED_Run.mode = mode;
	LED_Run.time_ms = time_ms;
	pthread_mutex_unlock(&led_token);
	return 0;
}
int RaspiExt_LED_Power_Config(unsigned char mode, int time_ms)
{
	if (mode != LED_MODE_ON && mode != LED_MODE_OFF && mode != LED_MODE_TOGGLE)
	{
#if RASPI_EXT_DEBUG
		printf("LED Power Config: Not own mode.\n");
#endif
		return -1;
	}
	pthread_mutex_lock(&led_token);
	LED_Power.mode = mode;
	LED_Power.time_ms = time_ms;
	pthread_mutex_unlock(&led_token);
	return 0;
}
int RaspiExt_LED_Hostx_Config(unsigned char mode, int time_ms, int host)
{
	if (mode != LED_MODE_ON && mode != LED_MODE_OFF && mode != LED_MODE_TOGGLE)
	{
#if RASPI_EXT_DEBUG
		printf("LED Hostx Config: Not own mode.\n");
#endif
		return -1;
	}
	if (pthread_mutex_lock(&led_token) == 0)
	{
		
		switch (host)
		{
		case 1:
			LED_Host1.mode = mode;
			LED_Host1.time_ms = time_ms;
			break;
		case 2:
			LED_Host2.mode = mode;
			LED_Host2.time_ms = time_ms;
			break;
		case 3:
			LED_Host3.mode = mode;
			LED_Host3.time_ms = time_ms;
			break;
		case 4:
			LED_Host4.mode = mode;
			LED_Host4.time_ms = time_ms;
			break;
		default:
		#if RASPI_EXT_DEBUG
			printf("RaspiExt LED Host Config: Invalid host %d.\n", host);
			#endif
			return -1;
		}
		pthread_mutex_unlock(&led_token);
	}
	else
	{
#if RASPI_EXT_DEBUG
		printf("LED Hostx Config: Cannot access led tocken.\n");
#endif 
	}
	return 0;
}


int RaspiExt_DestroyAll(void)
{
	pthread_cancel(led_run_thread);
	pthread_cancel(led_power_thread);
	pthread_cancel(led_host1_thread);
	pthread_cancel(led_host2_thread);
	pthread_cancel(led_host3_thread);
	pthread_cancel(led_host4_thread);
	pthread_cancel(power_off_button_thread);
	return 0;
}
int RaspiExt_WaitForExit(void)
{
	if (pthread_equal(pthread_self(), led_run_thread) == 0)
		pthread_join(led_run_thread, NULL);
	if (pthread_equal(pthread_self(), led_power_thread) == 0)
		pthread_join(led_power_thread, NULL);
	if (pthread_equal(pthread_self(), led_host1_thread) == 0)
		pthread_join(led_host1_thread, NULL);
	if (pthread_equal(pthread_self(), led_host2_thread) == 0)
		pthread_join(led_host2_thread, NULL);
	if (pthread_equal(pthread_self(), led_host3_thread) == 0)
		pthread_join(led_host3_thread, NULL);
	if (pthread_equal(pthread_self(), led_host4_thread) == 0)
		pthread_join(led_host4_thread, NULL);
	if (pthread_equal(pthread_self(), power_off_button_thread) == 0)
		pthread_join(power_off_button_thread, NULL);
	return 0;
}
int RaspiExt_DS1307_GetTime(Time_t * t)
{

	return 0;
}
int RaspiExt_DS1307_SetTime(Time_t t)
{

	return 0;
}
int RaspiExt_DS1307_GetDate(Date_t * d)
{

	return 0;
}
int RaspiExt_DS1307_SetDate(Date_t d)
{

	return 0;
}

int RaspiExt_Pin_Hostx_Active(int host)
{
	switch(host)
	{
	case 1:
		digitalWrite(HOST_SE1, LOW);
		break;
	case 2:
		digitalWrite(HOST_SE2, LOW);
		break;
	case 3:
		digitalWrite(HOST_SE3, LOW);
		break;
	case 4:
		digitalWrite(HOST_SE4, LOW);
		break;
	default:
#if RASPI_EXT_DEBUG
		printf("RaspiExt Host Active: Invalid host %d.\n", host);
#endif
		return -1;
	}
	return 0;
}
int RaspiExt_Pin_Hostx_Inactive(int host)
{
	switch(host)
	{
	case 1:
		digitalWrite(HOST_SE1, HIGH);
		break;
	case 2:
		digitalWrite(HOST_SE2, HIGH);
		break;
	case 3:
		digitalWrite(HOST_SE3, HIGH);
		break;
	case 4:
		digitalWrite(HOST_SE4, HIGH);
		break;
	default:
#if RASPI_EXT_DEBUG
		printf("RaspiExt Host Inactive: Invalid host %d.\n", host);
#endif
		return -1;
	}
	return 0;
}
