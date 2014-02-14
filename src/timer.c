/*
 * timer.c
 *
 *  Created on: 2014年2月14日
 *      Author: li
 *      http://baike.baidu.com/link?url=lAce9Jt2ZBzQ9ZfCnp2FLx_0ThMswgWlDwLN-ovQd53_iuUbb5MMJisT0fHYpCPVr2V7D4jBrjbVxYq_2C8sNK
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
void sigroutine(int signo) {
	switch (signo) {
	case SIGALRM:
		printf("Catch a signal -- SIGALRM \n");
		signal(SIGALRM, sigroutine);
		break;
	case SIGVTALRM:
		printf("Catch a signal -- SIGVTALRM \n");
		signal(SIGVTALRM, sigroutine);
		break;
	}
	return;
}
int main() {
	struct itimerval value, ovalue, value2; //(1)
	printf("process id is %d\n", getpid());
	signal(SIGALRM, sigroutine);
	signal(SIGVTALRM, sigroutine);
	{
		value.it_value.tv_sec = 1;
		value.it_value.tv_usec = 0;
		value.it_interval.tv_sec = 1;
		value.it_interval.tv_usec = 0;
		setitimer(ITIMER_REAL, &value, &ovalue); //(2)
	}
	{
		value2.it_value.tv_sec = 0;
		value2.it_value.tv_usec = 500000;
		value2.it_interval.tv_sec = 0;
		value2.it_interval.tv_usec = 500000;
		setitimer(ITIMER_VIRTUAL, &value2, &ovalue);
	}


    sigset_t           set;
	for (;;){
		sigsuspend(&set);//iao注释掉此行，SIGVTALRM起作用
	}
}
