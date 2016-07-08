/*
 * devices.c
 *
 *  Created on: Sep 17, 2015
 *      Author: kieu
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "devices.h"
#include "serial.h"
#include "raspi_ext.h"

#include <unistd.h>
#include "../wiringPi/wiringPi.h"
#include "kalmanfilter.h"
#include "server.h"
#include "logger.h"
#include <pthread.h>

#define DEV_DEBUG 0

struct Device dev_host[DEV_HOST_NUMBER];

#define BUFFER_SIZE 128
uint8_t send_buff[BUFFER_SIZE];
uint8_t recv_buff[BUFFER_SIZE];
//uint8_t data_buff[BUFFER_SIZE];

//uint8_t dev_buff[DEV_HOST_NUMBER][BUFFER_SIZE];

int waitforValidPacket(struct PhysicalPacket * packet, int timeout)
{
    struct PhysicalPacket * _packet = NULL;
    int _start_time = millis(); // compatible from arduino
    char _recv_buff[BUFFER_SIZE];
    char * _recv_index = _recv_buff;
    int _recv_len, serial_len;

    memset(recv_buff, 0, BUFFER_SIZE);

    while (1)
    {
        serial_len = Serial_Available();
        if (serial_len > 0)
        {
            Serial_GetData((char *)_recv_index, serial_len);
            _recv_index += serial_len;
        }

       _packet = (struct PhysicalPacket *) recv_buff;

        if (_packet->length < 7) // minimum header
        {
            // wrong packet
            memset(recv_buff, 0, _recv_len);
            _recv_index = recv_buff;
        }
        else if (_recv_len < _packet->length)
        {
            // not enough length
        }
        else if (checksum(_packet) != recv_buff[_recv_len-1])
        {
            // wrong checksum
            memset(recv_buff, 0, _recv_len);
            return -1;
        }
        else
        {
            // this step for validated packet only
            packet->length = _packet->length;
            packet->type = _packet->type;
            memset(&(packet->data), 0, sizeof (struct PacketData));
            memcpy(&(packet->data), &(_packet->data), _packet->length - 2);
            break;
        }

        if (millis() - _start_time > timeout)
        {
            log_write("Request packet timeout");
            return -2;
        }
    }

    return 0;
}

int sendControl(struct Device dev)
{

    return 0;
}

int queryData(struct Device * dev)
{
    int serial_len = 0;
    int i;
    int start_time = millis();

    /*
     *
     */
#if DEV_DEBUG
    printf("Query data for device: %2X\r\n", dev->dev_data.id);
#endif

    if (dev->last_try > 3)
    {
        // dev->dev_data.id = DEV_UNKNOWN;
        // dev->dev_data.data_type = DATA_TYPE_NULL;
        dev_host[i].dev_data.data[3] = 0;
        dev_host[i].dev_data.data[2] = 0;
        dev_host[i].dev_data.data[1] = 0;
        dev_host[i].dev_data.data[0] = 0;

    }
    dev->last_try++;

    // required physical packet had it length plus 1 for checksum
    memset(send_buff, 0, BUFFER_SIZE);
    struct PhysicalPacket * send_phy = (struct PhysicalPacket *)send_buff;
    memcpy(&(send_phy->data), &(dev->dev_data), sizeof(struct PacketData));
    send_phy->type = CMD_ULTRA_SONIC | CMD_TYPE_QUERY;
    send_phy->length = 2 // type and length
            + 1 // id
            + 4 // unique number
            + 1 // data type
            + get_data_length(dev->dev_data.data_type);
    ((uint8_t *)send_phy)[send_phy->length] = checksum(send_phy);
    Serial_SendMultiBytes((unsigned char *)send_phy, send_phy->length + 1); // send with checksum byte

    /*
     *
     */
#if DEV_DEBUG
    printf("Query Packet: ");
    for (i = 0; i < send_phy->length + 1; i++)
    {
        printf("%2X ", ((unsigned char *)send_phy)[i]);
    }
    printf("\r\n");
#endif

    
    struct PhysicalPacket * recv_phy = (struct PhysicalPacket *)recv_buff;
    
    if (waitforValidPacket(recv_phy, 10000) == 0) // 10ms timeout 
    {
        /*
         *
         */
#if DEV_DEBUG
        printf("Received Packet: ");
        for (i = 0; i < serial_len; i++)
        {
            printf("%2X ", recv_buff[i]);
        }
        printf("\r\n");
#endif

        if (GET_CMD_TYPE(recv_phy->type) == CMD_TYPE_ANSWER)
        {
            int i;

            // first, search if exist dev
            for (i = 0; i < DEV_HOST_NUMBER; i++)
            {
                if (memcmp(dev_host[i].dev_data.unique_number, recv_phy->data.unique_number, 4) == 0)
                {
                    memcpy(&(dev_host[i].dev_data), &(recv_phy->data), sizeof(struct PacketData));
#if DEV_DEBUG
                    printf("Receive data from: %2X.\r\n", dev_host[i].dev_data.id);
#endif
                    if (dev_host[i].dev_data.id == DEV_ULTRA_SONIC)
                    {
                        union float_s distance;
                        union int_s unique_number;
                        distance.b[0] = dev_host[i].dev_data.data[3];
                        distance.b[1] = dev_host[i].dev_data.data[2];
                        distance.b[2] = dev_host[i].dev_data.data[1];
                        distance.b[3] = dev_host[i].dev_data.data[0];
                        unique_number.b[0] = dev_host[i].dev_data.unique_number[3];
                        unique_number.b[1] = dev_host[i].dev_data.unique_number[2];
                        unique_number.b[2] = dev_host[i].dev_data.unique_number[1];
                        unique_number.b[3] = dev_host[i].dev_data.unique_number[0];
#if DEV_DEBUG
                        printf("Receive distance: %0.3f from ultra sonic: %d.\r\n",
                                distance.f, unique_number.n);
#endif
                    }
                    dev_host[i].last_try = 0;
                    break;
                }
            }
            if (i == 4) // fail on first step
            {
                // second, find free slot
                for (i = 0; i < DEV_HOST_NUMBER; i++)
                {
                    if (dev_host[i].dev_data.id == DEV_UNKNOWN)
                    {
                        memcpy(&(dev_host[i].dev_data), &(recv_phy->data), sizeof(struct PacketData));
                        dev_host[i].value = distance.f;
#if DEV_DEBUG
                        printf("Receive data from: %2X.\r\n", dev_host[i].dev_data.id);
                        if (dev_host[i].dev_data.id == DEV_ULTRA_SONIC)
                        {
                            union float_s distance;
                            union int_s unique_number;
                            distance.b[0] = dev_host[i].dev_data.data[3];
                            distance.b[1] = dev_host[i].dev_data.data[2];
                            distance.b[2] = dev_host[i].dev_data.data[1];
                            distance.b[3] = dev_host[i].dev_data.data[0];
                            unique_number.b[0] = dev_host[i].dev_data.unique_number[3];
                            unique_number.b[1] = dev_host[i].dev_data.unique_number[2];
                            unique_number.b[2] = dev_host[i].dev_data.unique_number[1];
                            unique_number.b[3] = dev_host[i].dev_data.unique_number[0];
                            printf("Receive distance: %0.3f from ultra sonic: %d.\r\n",
                                    distance.f, unique_number.n);
                        }
#endif
                        dev_host[i].last_try = 0;
                        break;
                    }
                }
            }

        }

        //        free(raw_buffer);
    }

    //    free(phy);
    return 0;
}

#include "SQLiteAPI.h"
long int record_count = 0;
extern sqlite3 * tmp_db;
extern int isConnectDB;

int Device_Polling(void) // thread
{
	struct timeval t;
    static kalman_state k;
    static bool firstline = true;
    char raw_data[128];

	// for (i = 0; i < DEV_HOST_NUMBER; i++)
	// {
	// 	printf("Query %d\n", i);
	// 	queryData(&dev_host[i]);
	// 	// usleep(20000);
	// }

    queryData(&dev_host[2]);
    record_count ++;


    if (firstline)
    {
        k = kalman_init(1, 15.0, 1, dev_host[2].value);
        firstline = false;
    }
    else
    {
        kalman_update(&k, dev_host[2].value);
        dev_host[2].value = k.x;
    }

    if (isConnectDB){
        sprintf(raw_data, "%0.4f, %0.4f, %0.4f, %0.4f",
                dev_host[0].value,
                dev_host[1].value,
                dev_host[2].value,
                dev_host[3].value);
        insert_sensor_value(tmp_db, isConnectDB, raw_data);
    }

    return 0;
}
int Device_Init(void)
{
    int i = 0;
    printf("Initial Sensor Host.\r\n");

    // for external components controller
    RaspiExt_Init();

    Serial_Init();
    for (i = 0; i < DEV_HOST_NUMBER; i++)
    {
        printf("Initial Sensor Host %d parameters.\r\n", i);
        dev_host[i].polling_control.enable = 0;
        dev_host[i].polling_control.time_poll_ms = 200;
        dev_host[i].polling_control.destroy = 0;
        dev_host[i].dev_data.id = DEV_ULTRA_SONIC;
        dev_host[i].dev_data.data_type = DATA_TYPE_NULL;
        dev_host[i].dev_data.unique_number[0] = 0;
        dev_host[i].dev_data.unique_number[1] = 0;
        dev_host[i].dev_data.unique_number[2] = 2;
        dev_host[i].dev_data.unique_number[3] = i;
        memset(dev_host[i].dev_data.data, 0, DATA_SIZE);
        dev_host[i].last_try = 0;
    }

    CreateServer(4);

    return 0;
}
