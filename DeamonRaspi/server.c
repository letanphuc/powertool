/*
 * server.c
 *
 *  Created on: May 14, 2016
 *      Author: Tra Quang Kieu
 */
#define SIMULATE_DATA 1

#include "server.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

#if SIMULATE_DATA
#else
#include "devices.h"
#endif
#include "SQLiteAPI.h"

pthread_t main_server_thread;
pthread_t client_threads;
int valid_thread_id = 0;
void * main_server_poll(void * params);
void * client_service(void * params);

const char * str_data_format = "{%d, '%s', %0.3f, %0.3f, %0.3f, %0.3f}\r"; // {dev1, dev2, dev3, dev4}
const char * cmd_get_data = "get data\r";
const char * cmd_start_record = "start record\r";
const char * start_record_res_msg = "OK, recording\r";
const char * cmd_stop_record = "stop record";
const char * stop_record_res_msg_format = "recorded file %s\r";

const char * cmd_copy_template_db = "cp /var/Raspido/raspido.db %s";
const char * tmp_filename_format = "/var/tmp/raspido_record%d%d%d%d%d%d";


struct recorder_controller
{
	int destroy;
	int poll_time;
	char recorded_name[64];
};
void * recorder(void * params)
{
	struct recorder_controller * control = (struct recorder_controller *) params;
	char copy_db[128];
	char tmp_filename[64];
	char raw_data[128];
    sqlite3 * tmp_db;
    int isConnectDB = 0;

	time_t t = time(NULL);
	struct tm tm;
	tm = *localtime(&t);

#if SIMULATE_DATA
    float f1, f2, f3, f4;
#else
	union float_s dev1_data, dev2_data, dev3_data, dev4_data;
#endif

	// create tmp file name
	memset(tmp_filename, 0, 64);
	sprintf(tmp_filename, tmp_filename_format,
			tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	// copy template database to temporary database
	memset(copy_db, 0, 128);
	sprintf(copy_db, cmd_copy_template_db, tmp_filename);
	// execute command
	printf("copy database cmd: %s\r\n", copy_db);
	system(copy_db);

	printf("recorded file name: %s\r\n", tmp_filename);
    strcpy(control->recorded_name, tmp_filename);
    isConnectDB = ConnectDB(&tmp_db, tmp_filename);

	for(;;)
	{
		if(control->destroy)
		{
			DisonnectDB(tmp_db, isConnectDB);
			// save database file
			break;
		}
		usleep(control->poll_time);

#if SIMULATE_DATA
        srand(time(NULL));
        f1 = rand()/1000;
        f2 = rand()/1000;
        f3 = rand()/1000;
        f4 = rand()/1000;

        memset(raw_data, 0, 128);
        sprintf(raw_data, "%0.3f, %0.3f, %0.3f, %0.3f", f1, f2, f3, f4);
#else
		dev1_data.b[0] = dev_host[0].dev_data.data[3];
		dev1_data.b[1] = dev_host[0].dev_data.data[2];
		dev1_data.b[2] = dev_host[0].dev_data.data[1];
		dev1_data.b[3] = dev_host[0].dev_data.data[0];

		dev2_data.b[0] = dev_host[1].dev_data.data[3];
		dev2_data.b[1] = dev_host[1].dev_data.data[2];
		dev2_data.b[2] = dev_host[1].dev_data.data[1];
		dev2_data.b[3] = dev_host[1].dev_data.data[0];

		dev3_data.b[0] = dev_host[2].dev_data.data[3];
		dev3_data.b[1] = dev_host[2].dev_data.data[2];
		dev3_data.b[2] = dev_host[2].dev_data.data[1];
		dev3_data.b[3] = dev_host[2].dev_data.data[0];

		dev4_data.b[0] = dev_host[3].dev_data.data[3];
		dev4_data.b[1] = dev_host[3].dev_data.data[2];
		dev4_data.b[2] = dev_host[3].dev_data.data[1];
		dev4_data.b[3] = dev_host[3].dev_data.data[0];

		memset(raw_data, 0, 128);
		sprintf(raw_data, "%0.3f, %0.3f, %0.3f, %0.3f", dev1_data.f, dev2_data.f, dev3_data.f, dev4_data.f);
#endif

		insert_sensor_value(tmp_db, isConnectDB, raw_data);
	}
	return NULL;
}

int CreateServer(int conn)
{
    printf("Create server\r\n");

	pthread_create(&main_server_thread, NULL, main_server_poll, (void *)conn);

	return 0;
}

void * main_server_poll(void * params)
{
	int _conn = (int)params;
	int sockfd, newsockfd;
    int clilen;
	struct sockaddr_in serv_addr, cli_addr;

	printf("Server listener started.\r\n");

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("ERROR opening socket. Server sleep forever.\r\n");
		for (;;)
			sleep(1);
	}
#if SIMULATE_DATA
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
#else
	bzero((char *) &serv_addr, sizeof(serv_addr));
#endif

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(5438);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
			sizeof(serv_addr)) < 0)
	{
		printf("ERROR on binding. Server sleep forever.\r\n");
		for (;;)
			sleep(1);
	}
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	for (;;)
	{
		newsockfd = accept(sockfd,
				(struct sockaddr *) &cli_addr,
				&clilen);
		if (newsockfd < 0)
		{
			printf("Server ERROR on accept.\r\n");
			for (;;)
				sleep(1);
		}
		else
			pthread_create(&client_threads, NULL, client_service, (void *)newsockfd);
	}
	close(sockfd);
	return NULL;
}


void * client_service(void * params)
{
	int clisockfd = (int)params;
	int n, data_count = 1;
	char recv_msg[40];
	char send_msg[128];
	char curr_time_str[20];

	pthread_t recorder_thread;
	struct recorder_controller ctrl;

	time_t t = time(NULL);
	struct tm tm;


	printf("Server service for client: %d.\r\n", clisockfd);
	for(;;)
	{
#if SIMULATE_DATA
        n = recv(clisockfd, recv_msg, 40, 0);
#else
        n = recv(clisockfd, recv_msg, 40, MSG_DONTWAIT);
#endif
		if (n < 0)
		{
//			printf("client %d: error on receive message.\r\n", clisockfd);
//			break;
		}
		else
		{
			if (memcmp (recv_msg, cmd_get_data, strlen(cmd_get_data)) == 0)
			{
				memset(send_msg, 0, 128);

                memset(curr_time_str, 0, 20);
                tm = *localtime(&t);

                sprintf(curr_time_str, "%d-%d-%d %d:%d:%d\n",
                        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

#if SIMULATE_DATA
                float f1, f2, f3, f4;
                srand(time(NULL));
                f1 = rand()/1000;
                f2 = rand()/1000;
                f3 = rand()/1000;
                f4 = rand()/1000;
#else
				union float_s dev1_data, dev2_data, dev3_data, dev4_data;

				dev1_data.b[0] = dev_host[0].dev_data.data[3];
				dev1_data.b[1] = dev_host[0].dev_data.data[2];
				dev1_data.b[2] = dev_host[0].dev_data.data[1];
				dev1_data.b[3] = dev_host[0].dev_data.data[0];

				dev2_data.b[0] = dev_host[1].dev_data.data[3];
				dev2_data.b[1] = dev_host[1].dev_data.data[2];
				dev2_data.b[2] = dev_host[1].dev_data.data[1];
				dev2_data.b[3] = dev_host[1].dev_data.data[0];

				dev3_data.b[0] = dev_host[2].dev_data.data[3];
				dev3_data.b[1] = dev_host[2].dev_data.data[2];
				dev3_data.b[2] = dev_host[2].dev_data.data[1];
				dev3_data.b[3] = dev_host[2].dev_data.data[0];

				dev4_data.b[0] = dev_host[3].dev_data.data[3];
				dev4_data.b[1] = dev_host[3].dev_data.data[2];
				dev4_data.b[2] = dev_host[3].dev_data.data[1];
				dev4_data.b[3] = dev_host[3].dev_data.data[0];
#endif

#if SIMULATE_DATA
                sprintf(send_msg, str_data_format, data_count++,
                        curr_time_str,
                        f1, f2, f3, f4);
#else
				sprintf(send_msg, str_data_format, data_count++,
						curr_time_str,
						dev1_data.f, dev2_data.f, dev3_data.f, dev4_data.f);
#endif
#if SIMULATE_DATA
                n = send(clisockfd, send_msg, strlen(send_msg), 0);
#else
				n = send(clisockfd, send_msg, strlen(send_msg), MSG_NOSIGNAL);
#endif
				if (n < 0)
				{
					printf("ERROR writing to socket.\r\n");
					break;
				}
			}
			if (memcmp (recv_msg, cmd_start_record, strlen(cmd_start_record)) == 0)
			{
#if SIMULATE_DATA
                n = send(clisockfd, "OK, recording\r", strlen("OK, recording\r"), 0);
#else
				n = send(clisockfd, "OK, recording\r", strlen("OK, recording\r"), MSG_NOSIGNAL);
#endif
				if (n < 0)
				{
					printf("ERROR writing to socket.\r\n");
					break;
				}
				ctrl.destroy = 0;
				ctrl.poll_time = 30000;
				memset(ctrl.recorded_name, 0, 64);
				pthread_create(&recorder_thread, NULL, recorder, (void *)&ctrl);
			}
			if (memcmp (recv_msg, cmd_stop_record, strlen(cmd_stop_record)) == 0)
			{
				ctrl.destroy = 1;
#if SIMULATE_DATA
                n = send(clisockfd, ctrl.recorded_name, strlen(ctrl.recorded_name), 0);
#else
				n = send(clisockfd, ctrl.recorded_name, strlen(ctrl.recorded_name), MSG_NOSIGNAL);
#endif
				if (n < 0)
				{
					printf("ERROR writing to socket.\r\n");
					break;
				}
			}
		}
		usleep(100);
	}

	printf("Close connection for client: %d.\r\n", clisockfd);
	close(clisockfd);

	return NULL;
}

