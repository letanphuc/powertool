/*
 * serial.h
 *
 *  Created on: Sep 18, 2015
 *      Author: kieu
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#ifdef _WIN32
   //define something for Windows (32-bit and 64-bit, this part is common)
#include <windows.h>
#include <stdio.h>
   #ifdef _WIN64
      //define something for Windows (64-bit only)
   #endif
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
         // iOS Simulator
    #elif TARGET_OS_IPHONE
        // iOS device
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
    #else
        // Unsupported platform
    #endif
#elif __linux
    // linux
#include "../wiringPi/wiringPi.h"
#include "../wiringPi/wiringSerial.h"
#elif __unix // all unices not caught above
    // Unix
#elif __posix
    // POSIX
#endif

int Serial_Init(void);
int Serial_DeInit(void);
int Serial_SendByte(unsigned char byte);
int Serial_SendMultiBytes(unsigned char * bytes, int len);
int Serial_Available(void);
int Serial_GetData(char * buffer, int len);


#endif /* SERIAL_H_ */
