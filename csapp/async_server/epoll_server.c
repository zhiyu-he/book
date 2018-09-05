#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>



#define SERVER_ADDR "0.0.0.1"
#define SERVER_PORT 9000
#define FD_SIZE     1000
#define MAX_SIZE    87380
#define EPOLLEVENTS 100
#define LISTENQ     5


static int socket_bind(const char* ip, int port);
static void do_epoll(int listenfd);
static void handle_events(int epollfd, struct epoll_event *events, int num, int listenfd, char *buf);

static void handle_accept(int epollfd, int listenfd);
static void do_read(int epollfd, int fd, char *buf);
static void do_write(int epollfd, int fd, char *buf);
static void add_event(int epollfd, int fd, int state);
static void modify_event(int epollfd, int fd, int state);
static void delete_event(int epollfd, int fd, int state);

int main(int argc, char **argv) {
    int listenfd;
    listenfd = socket_bind(SERVER_ADDR, SERVER_PORT);
    listen(listenfd, LISTENQ);
    do_epool(listenfd);
    return 0;
}

static int socket_bind(const char* ip, int port) {
    int listenfd;
    struct sockaddr_in servaddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        perror("socket error: ");
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INEF;
    inet_pton(AF_INET, ip, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);
    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        perror("bind error: ");
        exit(1);
    }
    return listenfd;
}

static void do_epoll(int listenfd) {
    int epoolfd;
    struct epoll_event events[EPOLLEVENTS];
    int ready_cnt;
    char buf[MAXSIZE];
    memset(buf, 0, MAXSIZE);
    epollfd = epoll_create(FD_SIZE);
    add_event(epollfd, listenfd, EPOLLIN);
    for ( ; ; ) {
        ready_cnt = epoll_wait(epollfd, evernts, EPOLLEVENTS, -1);
        handle_events(epollfd, events, ready_cnt, listenfd, buf);
    }
    close(epollfd);
}

static void handle_events(int epollfd, struct epoll_event *events, int num, int listenfd, char *buf) {
    int i;
    int fd;

    for (i = 0; i < num; i++) {
        fd = events[i].data.fd;
        if ((fd == listenfd) && (events[i].events & EPOLLIN))
            handle_accept(epollfd, listenfd);
        else if (events[i].events & EPOLLIN)
            do_read(epollfd, fd, buf);
        else if (events[i].events & EPOLLOUT)
            do_write(epollfd, fd, buf);
    }
}

static void handle_accept(int epollfd, int listenfd) {
    int clifd;
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen;

    clifd = accept(listenfd, (struct sockaddr*) &cliaddr, &cliaddrlen);
    if (clifd == -1)
        peerror("Accept error:");
    else {
        printf("Accept a new client: %s: %d\n",
                inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
        add_event(epollfd, clifd, EPOLLIN);
    }
}

static void do_read(int epollfd, int fd, char *buf) {
    int nread;
    nread = read(fd, buf, MAXSIZE);
    if (nread == -1) {
        perror("Read error:");
        delete_event(epollfd, fd, EPOLLIN);
        close(fd);
    }
    else if (nread == 0) {
        fprintf(stderr, "Client closed.\n");
        delete_event(epollfd, fd, EPOLLIN);
        close(fd);
    }
    else {
        modify_event(epollfd, fd, EPOLLOUT);
    }
}

static void do_write(int epollfd, int fd, char *buf) {
    int nwrite;
    nwrite = write(fd, buf, strlen(buf));
    if (nwrite == -1) {
        perror("Write error:");
        delete_event(epollfd, fd, EPOLLOUT);
        close(fd);
    }
    else
        modify_event(epollfd, fd, EPOLLIN);

    memset(buf, 0, MAXSIZE);
}

static void add_event(int epollfd, int fd, int state) {
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) < 0) {
        printf("Add event failed!\n");
    }
}

static void delete_event(int epollfd, int fd, int state) {
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    if (epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev) < 0) {
        printf("Delete event failed!\n");
    }
}

static void modify_event(int epollfd, int fd, int state) {
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    if (epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev) < 0) {
        printf("Modify event failed!\n");
    }
}
