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
		clock_t toc;
		clock_t tic = clock();
		Device_Polling();
		toc = clock();

        int s = 33333 - ((toc - tic) * 1000000 / CLOCKS_PER_SEC);

        usleep(s > 33333 ? 33333:s);
	}
	return 0;
}
