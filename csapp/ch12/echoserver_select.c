#include "../common/csapp.h"
#include "../common/rio.h"
/*
 * select函数会一直`阻塞`, 直到读集合中至少有一个描述符准备好可以读。
 * 如何理解阻塞呢?
 * 当且仅当一个从该描述符读取一个字节的请求不会阻塞时, 描述符k就表示准备好可以读了。
 *
 */


#define MAXLINE 1000

void echo(int connfd);
void command(void);

int main(int argc, char **argv) {

    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    fd_set read_set, ready_set;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = Open_listenfd(argv[1]);

    FD_ZERO(&read_set);
    FD_SET(STDIN_FILENO, &read_set);
    FD_SET(listenfd, &read_set);

    while (1) {
        ready_set = read_set;
        Select(listenfd+1, &ready_set, NULL, NULL, NULL);
        if (FD_ISSET(STDIN_FILENO, &ready_set))
            command();
        if (FD_ISSET(listenfd, &read_set)) {
            clientlen = sizeof(struct sockaddr_storage);
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
            echo(connfd);
            Close(connfd);
        }
    }
    return 0;
}

void command(void) {
    char buf[MAXLINE];
    if (!Fgets(buf, MAXLINE, stdin))
        exit(0);
    printf("%s", buf);
}
