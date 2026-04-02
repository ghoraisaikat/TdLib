#include "../include/uthread.h"
#include "../include/mut.h"
#include <time.h>

void counter(void) {
	for (int i = 0; i < 10; i++) {
		printf("Counter: %d (I'm still running!) \n", i);
		sleep(1);
	}
	return;
}

int main(){
	if (thread_init() == -1){
		printf("Thread system initialisation failed\n");
		return 1;
	}
	char buf[10];
	
	thread_create(counter);
	thread_start();
	
	printf("[MAIN] Enter something to 'block' this thread: ");
	fgets(buf, 10, stdin); // This blocks the entire thread
	printf("[MAIN] You entered: %s\n", buf);

	return 0;
}
