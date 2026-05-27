// handlers.c

#include "responses.h"
#include "pi.h"
#include "dtp.h"
#include "session.h"
#include "utils.h"
#include "logger.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

void handle_USER(const char *args) {
  ftp_session_t *sess = session_get();

  if (!args || strlen(args) == 0) {
    LOG_WRN("USER command received without arguments");
    safe_dprintf(sess->control_sock, MSG_501); // Syntax error in parameters
    return;
  }

  strncpy(sess->current_user, args, sizeof(sess->current_user) - 1);
  sess->current_user[sizeof(sess->current_user) - 1] = '\0';
  
  LOG_INF("User identity: %s", sess->current_user);
  safe_dprintf(sess->control_sock, MSG_331); // Username okay, need password
}

void handle_PASS(const char *args) {

  ftp_session_t *sess = session_get();

  if (sess->current_user[0] == '\0') {
    LOG_WRN("PASS attempted before USER");
    safe_dprintf(sess->control_sock, MSG_503); // Bad sequence of commands
    return;
  }

  if (!args || strlen(args) == 0) {
    safe_dprintf(sess->control_sock, MSG_501); // Syntax error in parameters
    return;
  }

  if (check_credentials(sess->current_user, (char *)args) == 0) {
    sess->logged_in = 1;
    LOG_INF("User '%s' logged in", sess->current_user);
    safe_dprintf(sess->control_sock, MSG_230); // User logged in
  } else {
    LOG_WRN("Failed login attempt for user: %s", sess->current_user);
    safe_dprintf(sess->control_sock, MSG_530); // Not logged in
    sess->current_user[0] = '\0'; // Reset user on failed login
    sess->logged_in = 0;
  }
}

void handle_QUIT(const char *args) {
  ftp_session_t *sess = session_get();
  (void)args; // unused

  LOG_INF("User %s disconnected", sess->current_user[0] ? sess->current_user : "Unknown");
  safe_dprintf(sess->control_sock, MSG_221); // 221 Goodbye.
  sess->current_user[0] = '\0'; // Close session
  close_fd(sess->control_sock, "client socket"); // Close socket
  sess->control_sock = -1;
}

void handle_SYST(const char *args) {
  ftp_session_t *sess = session_get();
  (void)args; // unused

  LOG_INF("SYST requested by %s", sess->current_user[0] ? sess->current_user : "anonymous");
  safe_dprintf(sess->control_sock, MSG_215); // 215 <system type>
}


void handle_TYPE(const char *args) {
  ftp_session_t *sess = session_get();
  (void)args;
  (void)sess;

  safe_dprintf(sess->control_sock, MSG_200);
  // Placeholder
}

void handle_PORT(const char *args) {
  ftp_session_t *sess = session_get();

  int port;
  char ip[INET_ADDRSTRLEN];

  if (get_info_from_port(args, ip, &port) < 0) {
    safe_dprintf(sess->control_sock, MSG_501);
    return;
  }

  memset(&sess->data_addr, 0, sizeof(sess->data_addr));
  sess->data_addr.sin_family = AF_INET;
  sess->data_addr.sin_port = htons(port);
  if (inet_pton(AF_INET, ip, &sess->data_addr.sin_addr) < 0) {
    safe_dprintf(sess->control_sock, MSG_501);
    return;
  }

  safe_dprintf(sess->control_sock, MSG_200);
}

void handle_RETR(const char *args) {
  ftp_session_t *sess = session_get();
  int fd;

  if (!sess->logged_in) {
    safe_dprintf(sess->control_sock, MSG_530);
    return;
  }

  if (!args || strlen(args) == 0) {
    safe_dprintf(sess->control_sock, MSG_501);
    return;
  }

  safe_dprintf(sess->control_sock, MSG_150);

  if ((fd = dtp_open(args)) < 0) {
    safe_dprintf(sess->control_sock, MSG_550, "file unavailable");
    return;
  }

  if (dtp_send(sess, fd) < 0) {
    safe_dprintf(sess->control_sock, MSG_451);
    return;
  }

  safe_dprintf(sess->control_sock, MSG_226);
}

void handle_STOR(const char *args) {
  ftp_session_t *sess = session_get();
  (void)args;
  (void)sess;

  // Placeholder
}

void handle_NOOP(const char *args) {
  ftp_session_t *sess = session_get();
  (void)args;

  safe_dprintf(sess->control_sock, MSG_200);
  // Placeholder
}
