#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/epoll.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_EVENTS 10
#define PORT 8080

//设置socket连接为非阻塞模式
void setnonblocking(int sockfd) {
    int opts;

    opts = fcntl(sockfd, F_GETFL);
    if(opts < 0) {
        perror("fcntl(F_GETFL)\n");
        exit(1);
    }
    opts = (opts | O_NONBLOCK);
    if(fcntl(sockfd, F_SETFL, opts) < 0) {
        perror("fcntl(F_SETFL)\n");
        exit(1);
    }
}

int main(){
    struct epoll_event ev, events[MAX_EVENTS];
    int addrlen, socket_fd, conn_sock, event_count, epoll_fd, fd, iFor1, nread;
    struct sockaddr_in local, remote;
    char buf[BUFSIZ];

    //创建listen socket
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("sockfd\n");
        exit(1);
    }
    setnonblocking(socket_fd);
    bzero(&local, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);;
    local.sin_port = htons(PORT);
    if( bind(socket_fd, (struct sockaddr *) &local, sizeof(local)) < 0) {
        perror("bind\n");
        exit(1);
    }
    listen(socket_fd, 20);

    epoll_fd = epoll_create(MAX_EVENTS);
    if (epoll_fd == -1) {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = socket_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (event_count == -1) {
            perror("epoll_pwait");
            exit(EXIT_FAILURE);
        }
        printf("[lyz]epoll_wait\r\n");

        for (iFor1 = 0; iFor1 < event_count; ++iFor1) {
            printf("[lyz]iFor1:%d,event_count:%d\r\n",iFor1,event_count);
            fd = events[iFor1].data.fd;
            if (fd == socket_fd) {
                printf("[lyz]fd == socket_fd\r\n");
                while ((conn_sock = accept(socket_fd,(struct sockaddr *) &remote,(size_t *)&addrlen)) > 0) {
                    setnonblocking(conn_sock);

                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = conn_sock;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_sock,&ev) == -1) {
                        perror("epoll_ctl: add");
                        exit(EXIT_FAILURE);
                    }

                }
                if (conn_sock == -1) {
                    if (errno != EAGAIN && errno != ECONNABORTED&& errno != EPROTO && errno != EINTR)
                        perror("accept");
                }
                continue;
            }
            if (events[iFor1].events & EPOLLIN) {
                printf("[lyz]EPOLLIN\r\n");
                int n = 0;
                while ((nread = read(fd, buf + n, BUFSIZ-1)) > 0) {
                    n += nread;
                }
                if (nread == -1 && errno != EAGAIN) {
                    perror("read error");
                }
                printf("[lyz]buf:%s\r\n",buf);

                ev.events = events[iFor1].events | EPOLLOUT;
                ev.data.fd = fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev) == -1) {
                    perror("epoll_ctl: mod");
                }
            }
            if (events[iFor1].events & EPOLLOUT) {
                printf("[lyz]EPOLLOUT\r\n");
                sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\nHello World", 11);
                int nwrite, data_size = strlen(buf);
                int n = data_size;
                while (n > 0) {
                    nwrite = write(fd, buf + data_size - n, n);
                    if (nwrite < n) {
                        if (nwrite == -1 && errno != EAGAIN) {
                            perror("write error");
                        }
                        break;
                    }
                    n -= nwrite;
                }
                close(fd);
            }
        }
    }

    return 0;
}
