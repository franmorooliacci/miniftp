#define _GNU_SOURCE
#include "dtp.h"
#include "session.h"
#include "config.h"
#include "logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>

int check_credentials(char *user, char *pass) {
  FILE *file;
  char *path = PWDFILE, *line = NULL, cred[100];
  size_t len = 0;
  int found = -1;

  // make the credential string
  sprintf(cred, "%s:%s", user, pass);

  // check if it is present in any ftpusers line
  file = fopen(path, "r");
  if (file == NULL) {
    LOG_ERROR("Error: no se pudo abrir el archivo de usuarios: %m");
    return -1;
  }

  while (getline(&line, &len, file) != -1) {
    strtok(line, "\n");
    if (strcmp(line, cred) == 0) {
      found = 0;
      break;
    }
  }

  fclose(file);
  if (line) free(line);
  return found;
}

int dtp_open(const char *filename) {
  int fp;

  if ((fp = open(filename, O_RDONLY)) < 0) {
    LOG_ERROR("file don't exist");
    return -1;
  }

  return fp;
}

int dtp_send(ftp_session_t *s, int fd) {
  char buffer[BUFFER_SIZE]; 
  int ds;

  if((ds = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    LOG_ERROR("");
    close(fd);
    return -1;
  }

  if(connect(ds, (struct sockaddr *)&s->data_addr, sizeof(s->data_addr)) < 0) {
    LOG_ERROR("");
    close(fd);
    return -1;
  }

  ssize_t br;
  while((br = read(fd, buffer, sizeof(buffer))) > 0) {
    ssize_t bs = write(ds, buffer, br);
    if (bs < 0 || br != bs) {
      LOG_ERROR("");
      close(fd);
      return -1;
    }
  }

  close(ds);
  close(fd);
  return 0;
}
