#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define LISTENQ 1024

// 利用getaddrinfo, 获取本机可用的addrinfo
//     struct addrinfo {
//             int ai_flags;           /* input flags */
//             int ai_family;          /* protocol family for socket */
//             int ai_socktype;        /* socket type */
//             int ai_protocol;        /* protocol for socket */
//             socklen_t ai_addrlen;   /* length of socket-address */
//             struct sockaddr *ai_addr; /* socket-address for socket */
//             char *ai_canonname;     /* canonical name for service location */
//             struct addrinfo *ai_next; /* pointer to next in list */
//     };

int open_clientfd(char *hostname, char *port)
{
    char abuf[10000], *addr; // for debug
    struct sockaddr_in *sinp; // for debug
    int clientfd;
    struct addrinfo hints, *listp, *p;
    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_flags |= AI_ADDRCONFIG;
    // 这个函数, 是不是就是返回可用的addrinfo结构?
    getaddrinfo(hostname, port, &hints, &listp);
    
    for (p = listp; p; p = p->ai_next)
    {
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;
		sinp = (struct sockaddr_in *)p->ai_addr;
        inet_ntop(AF_INET, &sinp->sin_addr, abuf,INET_ADDRSTRLEN);
        printf("addr-info: %s:%d\n", abuf, sinp->sin_port);

        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
            break;
        close(clientfd);
    }
    freeaddrinfo(listp);

    if (!p)
        return -1;
    else
        return clientfd;
}


int open_listenfd(char *port)
{
    struct addrinfo hints, *listp, *p;
    int listenfd, optval = 1;
    
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
    hints.ai_flags |= AI_NUMERICSERV;
    getaddrinfo(NULL, port, &hints, &listp);

    for (p = listp; p; p = p->ai_next)
    {
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;
        
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));

        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break;
        close(listenfd);
    }

    freeaddrinfo(listp);

    if (!p)
        return -1;
    
    if (listen(listenfd, LISTENQ) < 0) {
        close(listenfd);
        return -1;
    }
    return listenfd;
}
