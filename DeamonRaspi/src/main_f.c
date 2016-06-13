/*
 * main.c
 *
 *  Created on: Sep 22, 2015
 *      Author: kieutq
 */

#include <stdio.h>
#include <stdlib.h>

#include "devices.h"

#ifdef __linux
#include <unistd.h>
#include <signal.h>
#endif

#include <pthread.h>

void die(int err)
{
	printf("Program exit with code: %d\n", err);
	exit(err);
}


void * main_f (void * params)
{
	signal(SIGINT, die);
	printf("Sensor Host.\n");

	Device_Init();

	for (;;)
	{
		Device_Polling();
	}
	return 0;
}
