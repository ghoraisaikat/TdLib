#define _XOPEN_SOURCE 600
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ucontext.h>
#include <string.h>

#define endl "\n"
#define sp " "
#define debug() printf("I reached here!" endl)


const int TASKS = 2;
int curr_TASK = 0;
ucontext_t tasks[TASKS];
ucontext_t ctx_main;

void handleAlarm(int signum) {
	int curr_id = curr_TASK;
	int curr_next = (curr_id + 1) % 2;
	curr_TASK = curr_next;
	printf("Context switch baby!" endl);
	swapcontext(&tasks[curr_id], &tasks[curr_next]);
}

void func1(void) {
	while(1){
		printf("Bing!" endl);
		usleep(75000);
	}
}

void func2(void) {
	while(1){
		printf("Bong!" endl);
		usleep(75000);
	}
}

int main(void){
	signal(SIGALRM, handleAlarm);
	if (getcontext(&ctx_main) == -1) 
		perror("main, getcontext");

	void* STACK_f1 = malloc(4096);
	void* STACK_f2 = malloc(4096);
	if (getcontext(&tasks[0]) == -1) 
		perror("f1, getcontext");
	if (getcontext(&tasks[1]) == -1)
		perror("f2, getcontext");
	tasks[0].uc_stack.ss_sp 	= STACK_f1;
	tasks[0].uc_stack.ss_size 	= 4096;
	tasks[0].uc_link 		= NULL;

	tasks[1].uc_stack.ss_sp		= STACK_f2;
	tasks[1].uc_stack.ss_size 	= 4096;
	tasks[1].uc_link		= NULL;
	
	makecontext(&tasks[0], (void (*)(void)) func1, 0);
	makecontext(&tasks[1], (void (*)(void)) func2, 0);
	

	struct itimerval timer;
	timer.it_interval.tv_usec	= 150000;
	timer.it_interval.tv_sec	= 0;
	timer.it_value.tv_usec		= 150000;
	timer.it_value.tv_sec		= 0;
	
	if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
		perror("setitimer");
	swapcontext(&ctx_main, &tasks[0]);
	printf("Came back from a long day's work!" endl);
	return 0;
}
