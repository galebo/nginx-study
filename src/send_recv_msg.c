#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

/* ************************************************************************
 *       Filename:  msg.c
 *    Description:
 *        Version:  1.0
 *        Created:  2011年08月16日 20时07分52秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (),
 *        Company:
 * ************************************************************************/
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <arpa/inet.h>

void sendmsg() {
	char line[15] = "Hello World!";
	struct iovec iov;
	struct sockaddr_in receiver_addr;
	struct msghdr msg;

	int sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	receiver_addr.sin_family = AF_INET;
	receiver_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	receiver_addr.sin_port = htons(5000);

	msg.msg_name = &receiver_addr;
	msg.msg_namelen = sizeof(receiver_addr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_iov->iov_base = line;
	msg.msg_iov->iov_len = sizeof(line);
	msg.msg_control = 0;
	msg.msg_controllen = 0;
	msg.msg_flags = 0;
	sendmsg(sock_fd, &msg, 0);
	close(sock_fd);
}

void SendMsgRecvMsg()
{
   int sock_fd;
   socklen_t sender_len;
   struct msghdr msg;
   struct iovec iov;
   struct sockaddr_in receiver_addr,sender_addr;
   char line[10];

   sock_fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
   if(sock_fd < 0)
   {
        perror("socket error");
        close(sock_fd);
        exit(0);
   }

   receiver_addr.sin_family = AF_INET;
   receiver_addr.sin_port = htons(5000);
   receiver_addr.sin_addr.s_addr = htonl(INADDR_ANY);

   if(bind(sock_fd,(struct sockaddr*)&receiver_addr,sizeof(receiver_addr)) < 0)
   {
        perror("bind error");
        close(sock_fd);
        exit(0);
   }

   sender_len = sizeof(sender_addr);

   msg.msg_name = &sender_addr;
   msg.msg_namelen = sender_len;
   msg.msg_iov = &iov;
   msg.msg_iovlen = 1;
   msg.msg_iov->iov_base = line;
   msg.msg_iov->iov_len = 10;
   msg.msg_control = 0;
   msg.msg_controllen = 0;
   msg.msg_flags = 0;

   printf("wait...\n");
   recvmsg(sock_fd,&msg,0);
   printf("wait is over!\n");
   printf("the msg is %s\n",(char *)(msg.msg_iov->iov_base));
   close(sock_fd);
}

int main(int argc ,char *argv[])
{
    SendMsgRecvMsg();
    return 0;
}
