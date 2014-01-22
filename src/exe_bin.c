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
int parentId = 1;
static void ngx_execute_proc(char *data, int pid, char * const *argv) {
	if (execve("/home/li/WORK/git/nginx-study/src/exe_bin", argv, NULL) == -1) {
		printf("exe %s error,pid:%d\r\n", data, pid);
	}
	printf("exe %s ok,pid:%d\r\n", data, pid);
	exit(1);
}

int main(int argc, char * const *argv) {
	int ngx_pid = getpid();
	printf("exe pid:%d\r\n", ngx_pid);
	int next=1;
	while (next) {
		printf("please input:");
		int ch = getchar();
		printf("%c\r\n",ch);
		switch (ch) {
		case 'n': {
			int pid = fork();

			switch (pid) {

			case -1:
				printf("fork() failed while spawning \r\n");
				return -1;

			case 0:
				parentId = parentId + 1;
				ngx_pid = getpid();
				ngx_execute_proc("son", ngx_pid, argv);
				break;

			default:
				break;
			}
		}
			break;
		case 'e':
			next=0;
			break;
		}
	}

	return 0;
}
