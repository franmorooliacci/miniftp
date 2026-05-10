#include "logger.h"

void init_logger(void) {
    // LOG_PID: Include process ID in each message
    // LOG_NDELAY: Open connection immediately
    // LOG_DAEMON: Standard facility for system background processes
    openlog("miniftp", LOG_PID | LOG_NDELAY, LOG_DAEMON);
}

void close_logger(void) {
    closelog();
}
