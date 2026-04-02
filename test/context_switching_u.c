#include <time.h>
#include "../include/uthread.h"
#include "../include/mut.h"

#define YIELDS 10000

void dummy_func(void) {
	for (int i = 0; i < YIELDS; i++) {
		thread_yield();
	}
	return;
}

int main(void) {
	if (thread_init() == -1) {
		printf("Thread system initialisation error\n");
		return 1;
	}
	struct timespec start, end;
	thread_create(dummy_func);

	clock_gettime(CLOCK_MONOTONIC, &start);
	thread_start();
	clock_gettime(CLOCK_MONOTONIC, &end);
    	
	double total = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    	printf("Total time for %d context switches: %f seconds\n", YIELDS * 2, total);
    	return 0;
}
