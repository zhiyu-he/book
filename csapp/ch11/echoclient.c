#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "base.h"
#include "rio.h"


int main(int argc, char **argv)
{
    int clientfd;
    char *host, *port, buf[1000];
    rio_t rio;   
    if (argc != 3) {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    port = argv[2];

    clientfd = open_clientfd(host, port);
    rio_readinitb(&rio, clientfd);

    while (fgets(buf, 1000, stdin) != NULL) {
        rio_writen(clientfd, buf, strlen(buf));
        rio_readlineb(&rio, buf, 1000);
        fputs(buf, stdout);
    }
    close(clientfd);

	exit(0);
}



