#include <stdio.h>


/*
 * 进程的优劣, 对于在父子进程之间共享`状态`信息, 进程有一个非常清晰的模型: 恭喜那个文件表, 但是不共享用户地址空间.
 *
 * 进程控制和IPC的开销比较大.
 * IPC机制
 *   1. 同主机, waitpid && signal
 *   2. 不同主机, socket
 *
 */

void echo(int connfd);

void sigchld_hanlder(int sig) {
    while (waitpid(-1, 0, WNOHANG) > 0);
    return;
}

int main(int argc, char **argv) {
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    Signal(SIGCHLD, sigchld_handler);
    listenfd = Open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
        if (Fork() == 0) {
            Close(listenfd);
            echo(connfd);
            Close(connfd);
            exit(0);
        }
        Close(connfd);
    }
    return 0;
}


