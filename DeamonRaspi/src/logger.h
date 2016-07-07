/*
 * logger.h
 *
 *  Created on: Jul 07, 2016
 *      Author: tqkieu
 */
#ifndef __logger_h_
#define __logger_h_

/* log mode */
#define LOG_MODE_FILE_ONLY      1
#define LOG_MODE_CONSOLE_ONLY   2
#define LOG_MODE_ANY            3

int log_config(const char * log_name, int log_mode);
int log(const char * msg);
int log_close(void);
int log_open(void);

#endif
