#define _POSIX_C_SOURCE 200809L
#include "server.h"
#include "logger.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void close_fd(int fd, const char *label) {

  if (close(fd) < 0) {
    LOG_ERROR("Error closing %s: %m", label);
  }
}

ssize_t safe_dprintf(int fd, const char *format, ...) {
  va_list args;
  va_start(args, format);
  ssize_t ret = vdprintf(fd, format, args);
  va_end(args);

  if (ret < 0) {
    LOG_ERROR("dprintf error: %m");
  }
  return ret;
}

int get_info_from_port(const char *cmd, char *ip, int *port){
  int h1, h2, h3, h4;
  int p1, p2;

  if(cmd == NULL || ip == NULL || port == NULL)
    return -1;

  if(sscanf(cmd,
            "%d,%d,%d,%d,%d,%d",
            &h1, &h2, &h3, &h4,
            &p1, &p2) != 6)
  {
    return -1;
  }

  if(h1 < 0 || h1 > 255 ||
     h2 < 0 || h2 > 255 ||
     h3 < 0 || h3 > 255 ||
     h4 < 0 || h4 > 255 ||
     p1 < 0 || p1 > 255 ||
     p2 < 0 || p2 > 255)
  {
    return -1;
  }

  sprintf(ip, "%d.%d.%d.%d", h1, h2, h3, h4);

  *port = (p1 * 256) + p2;

  return 0;
} 
