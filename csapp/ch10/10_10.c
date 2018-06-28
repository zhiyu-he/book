#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define RIO_BUFFSIZE 8192
#define MAXLINE 10

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


// Q: 如何统计一个文本的行数?
// A: 一次readline, 避免read陷入内核的次数

int main(int argc, char **argv)
{

  	FILE *stream;
    char *in;
    if (argc == 2)
        in = argv[1];
  		stream = fmemopen(in, sizeof(in), "r");

    int n ;
    rio_t rio;
    char buf[MAXLINE];
    rio_readinitb(&rio, STDIN_FILENO);
    while((n = rio_readlineb(&rio, buf, MAXLINE)) != 0)
        rio_writen(STDOUT_FILENO, buf,n);
    return 0;
}

// 成功返回读取的数量, 错误返回-1, EOF返回0
int rio_readn(int fd, void *userbuf, int n)
{
    // 读取可能存在多次, 因此终止条件为, EOF or 已经读取了n个字节
    int left_n = n;
    int r = 0;
    char *buf = userbuf;
    while(left_n > 0)
    {
        r = read(fd, buf, n);
        if (r < 0) {
            if (r == EINTR)    // 这里是一个系统中断, recall-again
                r = 0;
            else
                return -1;
        } else if (r == 0) {
            break;
        }
        left_n -= r;
        buf += r;
    }
    return n - left_n;
}

int rio_writen(int fd, void *userbuf, int n)
{
    int left_n = n;
    int w = 0;
    char *buf = userbuf;
    while(left_n > 0)
    {
        w = write(fd, buf, left_n);
        if (w < 0) {
            if (w == EINTR)
                w = 0;
            else
                return -1;
        }
        left_n -= w;
        buf += w;
    }
    return n - left_n;
}

void rio_readinitb(rio_t *rp, int fd)
{
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

int rio_read(rio_t *rp, char *user_buf, int n)
{
    int cnt = 0;
    // 当rp->rio_cnt <= 0的时候, 意味着缓冲区为空, 需要一次系system-call
    while(rp->rio_cnt <= 0)
    {
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if (rp->rio_cnt < 0)
        {
            if (rp->rio_cnt != EINTR)
                return -1;
        }
        else if (rp->rio_cnt == 0)  // EOF
            return 0;
        else
            rp->rio_bufptr = rp->rio_buf;
    }

    cnt = n;
    if (rp->rio_cnt < n)
        cnt = rp->rio_cnt;
    memcpy(user_buf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}

int rio_readlineb(rio_t *rp, void *userbuf, int maxlen)
{
    int n, rc;
    char c, *bufp = userbuf;
    for (n = 1; n < maxlen; n++) {
        if ((rc = rio_read(rp, &c, 1)) == 1) {
           *bufp++ = c;
           if (c == '\n')
               break;
        } else if (rc == 0) {
            if (n == 1)
                return 0;
            else
                break;
        } else
            return -1;
    }
    *bufp = 0;
    return n;
}

int rio_readnb(rio_t *rp, void *userbuf, int n)
{
    int left_n = n;
    int r;
    char *bufp = userbuf;
    while(left_n > 0)
    {
        r = rio_read(rp, bufp, left_n);
        if (r < 0)
        {
            if (r == EINTR)
                r = 0;
            else
                return-1;
        }
        else if (r == 0)
            break;
        left_n -= r;
        bufp += r;
    }
    return n - left_n;
}
