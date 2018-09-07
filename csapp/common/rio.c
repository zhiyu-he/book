#include "rio.h"

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


void Rio_readinitb(rio_t *rp, int fd)
{
    rio_readinitb(rp, fd);
}

ssize_t Rio_readlineb(rio_t *rp, void *userbuf, size_t maxlen)
{
    ssize_t rc;
    rc = rio_readlineb(rp, userbuf, maxlen);
    return rc;
}

void Rio_writen(int fd, void *userbuf, int n)
{
    rio_writen(fd, userbuf, n);
    //if (rio_writen(fd, userbuf, n) != n)
        //unix_error("Rio_writen error");
}

void echo(int connfd)
{
    size_t n;
    char buf[1000];
    rio_t rio;
    rio_readinitb(&rio, connfd);
    while ((n = rio_readlineb(&rio, buf, 1000)) != 0) {
        printf("server received %d bytes\n", (int)n);
        rio_writen(connfd, buf, n);
    }
}
