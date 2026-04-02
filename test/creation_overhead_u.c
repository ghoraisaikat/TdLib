#include <time.h>
#include "../include/uthread.h"
#include "../include/mut.h"

#define NUM_THREADS 10000

void dummy_func(void) {
	thread_exit();
}

int main(){
	if (thread_init() == -1) {
		printf("Thread system initialization failed\n");
		return 1;
	}

	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);

	for (int i = 0; i < NUM_THREADS; i++)
		thread_create(dummy_func);
	thread_start();

	clock_gettime(CLOCK_MONOTONIC, &end);
    	double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    	printf("One-to-One Creation: %f seconds for %d threads\n", time_taken, NUM_THREADS);
    	printf("Average per thread: %f microseconds\n", (time_taken / NUM_THREADS) * 1e6);
    	return 0;
}
