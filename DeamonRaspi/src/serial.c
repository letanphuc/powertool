/*
 * serial.c
 *
 *  Created on: Sep 18, 2015
 *      Author: kieu
 */

#include "serial.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SERIAL_DIR_PIN 		23
#define SERIAL_DIR_OUTPUT	{digitalWrite(SERIAL_DIR_PIN, HIGH); usleep(1);}
#define SERIAL_DIR_INPUT	{usleep(10); digitalWrite(SERIAL_DIR_PIN, LOW);}


#ifdef _WIN32
LPCSTR  SerialDevName = "COM3";
HANDLE hSerial;
#elif __linux
#include <unistd.h>
const char * SerialDevName = "/dev/ttyAMA0";

const unsigned int SerialBaudrate = 115200;

int serial_fd;
#endif


int Serial_Init(void)
{
#ifdef _WIN32
	hSerial = CreateFile(SerialDevName,
			GENERIC_READ | GENERIC_WRITE,
	    0,
	    0,
	    OPEN_EXISTING,
	    0,
	    0);
	if (hSerial == INVALID_HANDLE_VALUE) {
	    //call GetLastError(); to gain more information
		printf("Cannot open %s.\r\n", (char *) SerialDevName);
		return -1;
	}

	DCB dcbSerialParams = {0};
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (!GetCommState(hSerial, &dcbSerialParams)) {
	     //could not get the state of the comport
	}
	dcbSerialParams.BaudRate=BAUD_115200;
	dcbSerialParams.ByteSize=8;
	dcbSerialParams.StopBits=ONESTOPBIT;
	dcbSerialParams.Parity=NOPARITY;
	if(!SetCommState(hSerial, &dcbSerialParams)){
	     //analyse error
		printf("Serial parameter cannot initialized.\r\n");
		return -1;
	}
	COMMTIMEOUTS timeouts={0};
	timeouts.ReadIntervalTimeout=50;
	timeouts.ReadTotalTimeoutConstant=50;
	timeouts.ReadTotalTimeoutMultiplier=10;
	timeouts.WriteTotalTimeoutConstant=50;
	timeouts.WriteTotalTimeoutMultiplier=10;
	if(!SetCommTimeouts(hSerial, &timeouts)){
	    //handle error
		printf("Serial timeout cannot set.\r\n");
	}
	return 0;
#elif __linux
	serial_fd = serialOpen(SerialDevName, SerialBaudrate);
	if (serial_fd < 0)
	{
		printf("Cannot open serial.");
		return -1;
	}
	serialFlush(serial_fd);

	pinMode(SERIAL_DIR_PIN, OUTPUT);
	SERIAL_DIR_INPUT;

	return serial_fd;
#endif
}

int Serial_DeInit(void)
{
#ifdef _WIN32
	CloseHandle(hSerial);
#elif __linux
	serialClose(serial_fd);
#endif

	return 0;
}

int Serial_SendByte(unsigned char byte)
{
	SERIAL_DIR_OUTPUT;

#ifdef _WIN32
	DWORD dwBytesRead = 0;
	if(!WriteFile(hSerial, &byte, 1, &dwBytesRead, NULL))
	{
		printf("Cannot write byte %X to serial.\r\n", byte);
	}
	return dwBytesRead;
#elif __linux
	serialPutchar(serial_fd, byte);
	usleep(8000000/SerialBaudrate);
#endif

	SERIAL_DIR_INPUT;

	return 0;
}

int Serial_SendMultiBytes(unsigned char * bytes, int len)
{
	SERIAL_DIR_OUTPUT;

#ifdef _WIN32
	DWORD dwBytesRead = 0;
	if(!WriteFile(hSerial, bytes, len, &dwBytesRead, NULL))
	{
		printf("Cannot write data to serial.\r\n");
	}
	return dwBytesRead;
#elif __linux
	int i;
	for (i = 0; i < len; i++)
	{
		serialPutchar(serial_fd, bytes[i]);
		usleep(8000000/SerialBaudrate);
	}
#endif

	SERIAL_DIR_INPUT;

	return 0;
}

int Serial_Available(void)
{
#ifdef _WIN32
	return 0;
#elif __linux
	return serialDataAvail(serial_fd);
#endif
}

int Serial_GetData(char * buffer, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
#ifdef _WIN32
		DWORD dwBytesRead = 0;
		if(!ReadFile(hSerial, buffer, len, &dwBytesRead, NULL)){
			//handle error
			printf("Cannot read data from serial.\r\n");
		}
		return dwBytesRead;
#elif __linux
		buffer[i] = serialGetchar(serial_fd);
#endif
	}
	
	//serialFlush(serial_fd);
	return i;
}


