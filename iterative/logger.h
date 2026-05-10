#ifndef LOGGER_H
#define LOGGER_H

#include <syslog.h>

#define LOG_INF(fmt, ...) syslog(LOG_INFO, fmt, ##__VA_ARGS__)
#define LOG_WRN(fmt, ...) syslog(LOG_WARNING, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)  syslog(LOG_ERR, fmt, ##__VA_ARGS__)

void init_logger(void);
void close_logger(void);

#endif
