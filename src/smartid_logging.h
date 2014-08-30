/*
 * logging.h
 *
 *  Created on: Aug 24, 2014
 *      Author: jkrauss
 */

#ifndef SMARTID_LOGGING_H_
#define SMARTID_LOGGING_H_

void smartid_log_open(void);
void smartid_log_close(void);

void smartid_log_use_syslog(int use);
void smartid_log_use_stderr(int use);
void smartid_log_debug_lvl(int lvl);

void smartid_log_syserror(const char * fmt, ...);
void smartid_log_error(const char * fmt, ...);
void smartid_log_warning(const char * fmt, ...);
void smartid_log_info(const char * fmt, ...);
void smartid_log_debug(const char * fmt, ...);

#endif /* SMARTID_LOGGING_H_ */
