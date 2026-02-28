#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ucontext.h>

#define endl "\n"
#define sp " "


ucontext_t ctx_f1, ctx_f2, ctx_main;

void func1(void) {

		printf("Bing!" endl);
		usleep(100000);
		swapcontext(&ctx_f1, &ctx_f2);
		swapcontext(&ctx_f1, &ctx_main);	
}

void func2(void) {
	
		printf("Bong!" endl);
		usleep(100000);
		swapcontext(&ctx_f2, &ctx_f1);
}

int main(void){
	if (getcontext(&ctx_main) == -1) 
		perror("main, getcontext");
	
	void* STACK_f1 = malloc(4096);
	void* STACK_f2 = malloc(4096);
	if (getcontext(&ctx_f1) == -1) 
		perror("f1, getcontext");
	if (getcontext(&ctx_f2) == -1)
		perror("f2, getcontext");

	ctx_f1.uc_stack.ss_sp 	= STACK_f1;
	ctx_f1.uc_stack.ss_size = 4096;
	ctx_f1.uc_link 		= NULL;

	ctx_f2.uc_stack.ss_sp	= STACK_f2;
	ctx_f2.uc_stack.ss_size = 4096;
	ctx_f2.uc_link		= NULL;

	makecontext(&ctx_f1, (void (*)(void)) func1, 0);
	makecontext(&ctx_f2, (void (*)(void)) func2, 0);

	swapcontext(&ctx_main, &ctx_f1);
	printf("Came back from a long day's work!" endl);
	return 0;
}
