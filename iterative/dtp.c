#define _GNU_SOURCE
#include "dtp.h"
#include "logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
