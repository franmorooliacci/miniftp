#pragma once

#include "session.h"

#define PWDFILE "../ftpusers"

int check_credentials(char *user, char *pass);

int dtp_open(const char *filename);

int dtp_send(ftp_session_t *s, int fd);
