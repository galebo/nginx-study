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

#define nevents 10
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

int socket_init(){
    //创建listen socket
	int socket_fd;
	struct sockaddr_in local;
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

    return socket_fd;
}
int ep;
struct epoll_event event_list[nevents];
void epoll_init(int socket_fd){
	struct epoll_event ev;
    /*nginx 创一个epoll对象，容量为总连接数的一半 ep = epoll_create(cycle->connection_n / 2);*/
	ep = epoll_create(nevents);
    if (ep == -1) {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = socket_fd;
    if (epoll_ctl(ep, EPOLL_CTL_ADD, socket_fd, &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }
}
void epoll_process_events(int socket_fd){

    struct epoll_event ev;
    int addrlen,  now_fd, i, nread;
    struct sockaddr_in  remote;
    char buf[BUFSIZ];

    while(1) {
    	int events = epoll_wait(ep, event_list, nevents, -1);
        /*events = epoll_wait(ep, event_list, (int) nevents, timer);*/
        if (events == -1) {
            perror("epoll_pwait");
            exit(EXIT_FAILURE);
        }
        printf( "[lyz]==========================\r\n"
        		"[lyz]epoll_wait\r\n");

        for (i = 0; i < events; ++i) {
            printf("[lyz]iFor1:%d,event_count:%d\r\n",i,events);

            if (event_list[i].data.fd == socket_fd) {
                printf("[lyz]now_fd == socket_fd\r\n");
                int conn_sock;
                while ((conn_sock = accept(socket_fd,(struct sockaddr *) &remote,(size_t *)&addrlen)) > 0) {
                    setnonblocking(conn_sock);

                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = conn_sock;
                    if (epoll_ctl(ep, EPOLL_CTL_ADD, conn_sock,&ev) == -1) {
                        perror("epoll_ctl: add");
                        exit(EXIT_FAILURE);
                    }

                }
                if (conn_sock == -1) {
                    if (errno != EAGAIN && errno != ECONNABORTED&& errno != EPROTO && errno != EINTR)
                        perror("accept");
                }
            }else {
				if (event_list[i].events & EPOLLIN) {
					printf("[lyz]EPOLLIN\r\n");
					int n = 0;
					while ((nread = read(event_list[i].data.fd, buf + n, BUFSIZ - 1)) > 0) {
						n += nread;
					}
					if (nread == -1 && errno != EAGAIN) {
						perror("read error");
					}
					printf("[lyz]buf:%s\r\n", buf);

					ev.events = event_list[i].events | EPOLLOUT;
					ev.data.fd = event_list[i].data.fd;
					if (epoll_ctl(ep, EPOLL_CTL_MOD, event_list[i].data.fd, &ev) == -1) {
						perror("epoll_ctl: mod");
					}
				}
				if (event_list[i].events & EPOLLOUT) {
					printf("[lyz]EPOLLOUT\r\n");
					sprintf(buf,"HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\nHello World",11);
					int nwrite, data_size = strlen(buf);
					int n = data_size;
					while (n > 0) {
						nwrite = write(event_list[i].data.fd, buf + data_size - n, n);
						if (nwrite < n) {
							if (nwrite == -1 && errno != EAGAIN) {
								perror("write error");
							}
							break;
						}
						n -= nwrite;
					}
					close(event_list[i].data.fd);
				}
			}


        }
    }
}
int main(){
    int socket_fd=socket_init();
    epoll_init(socket_fd);
    epoll_process_events(socket_fd);
    return 0;
}
