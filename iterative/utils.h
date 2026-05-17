#pragma once

#include <sys/types.h>

void close_fd(int fd, const char *label);
ssize_t safe_dprintf(int fd, const char *format, ...);
int get_info_from_port(const char *cmd, char *ip, int *port);
