#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    // for debug
    struct sockaddr_in *sp;
    char debug_buf[1000];
    // --------
    struct addrinfo *p, *listp, hints;
    char buf[1000];
    int rc, flags;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <domain name>\n", argv[0]);
        exit(0);
    }
    // Get a list of addrinfo records
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if ((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
        exit(1);
    }
    flags = NI_NUMERICHOST;
    int times = 0;
    for (p = listp; p; p = p->ai_next) {
        sp = (struct sockaddr_in *)p->ai_addr; // 是一个socketaddr *, 可以强转称socketaddr_in
		inet_ntop(AF_INET, &sp->sin_addr, debug_buf, INET_ADDRSTRLEN);
        printf("times: addr: %s:%d\n", debug_buf, sp->sin_port);
        for (int i =0; i < p->ai_addrlen; i++)
            printf("%c ", p->ai_addr->sa_data[i]);
        printf("\n");
        getnameinfo(p->ai_addr, p->ai_addrlen, buf, 1000, NULL, 0, flags);
        printf("%s\n", buf);
    }
    freeaddrinfo(listp);
    exit(0);
}
