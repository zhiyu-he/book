#ifndef __CSAPP_H__
#define __CSAPP_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct sockaddr SA;

#define LISTENQ 1024


void unix_error(char *msg);
void app_error(char *msg);

/* sockets interface wrappers */
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);
void Connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
void Listen(int s, int backlog);

/* Unix I/O wrappers */
void Close(int fd);

/* Standard I/O wrappers */
char *Fgets(char *ptr, int n, FILE *stream);

/* Client/server helper functions */
int open_clientfd(char *hostname, int portno);
int open_listenfd(char *portno);

/* Wrappers for client/server helper functions */
int Open_clientfd(char *hostname, char *port);
int Open_listenfd(char *port); 

int Select(int  n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

/* Signal wrappers */
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);



/* Process control wrappers */
pid_t Fork(void);

#endif
