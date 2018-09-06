#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "base.h"
#include "rio.h"

typedef struct sockaddr SA;

void echo(int connfd);


int main(int argc, char **argv)
{
	int listenfd, connfd;
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;	// enough space for any address
	char client_hostname[1000], client_port[1000];
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}
	
	listenfd = open_listenfd(argv[1]);
	while (1) {
		clientlen = sizeof(struct sockaddr_storage);
		connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
		getnameinfo((SA *) &clientaddr, clientlen, client_hostname, 1000, client_port, 1000, 0);
		printf("Connected to (%s, %s)\n", client_hostname, client_port);
		echo(connfd);
		close(connfd);
	}
	exit(0);
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
