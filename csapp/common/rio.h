#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define RIO_BUFFSIZE 8192
#define MAXLINE 1000

typedef struct {
    int rio_fd;
    int rio_cnt;
    char *rio_bufptr;
    char rio_buf[RIO_BUFFSIZE];
} rio_t;


/*
 * STDIN_FILENO
 * STDOUT_FILENO
 * STDERR_FILENO
 *
 */

// base:
// int read(fd, *buf, n) read n byte from fd to fd
// int write(fd, *buf, n) write n byte to fd

// wrapper
// int rio_read(fd, *buf, n);
// int rio_write(fd, *buf,n);
//

// 成功返回传送的字节数, 若EOF则为0(针对rio_read), 出错则为-1
int rio_readn(int fd, void *userbuf, int n);
int rio_writen(int fd, void *userbuf, int n);

void rio_readinitb(rio_t *rp, int fd);

int rio_readlineb(rio_t *rp, void *userbuf, int max_len);
int rio_readnb(rio_t *rp, void *userbuf, int n);


void echo(int connfd);

void Rio_readinitb(rio_t *rp, int fd);
void Rio_writen(int fd, void *userbuf, int n);
ssize_t Rio_readlineb(rio_t *rp, void *userbuf, size_t maxlen);


