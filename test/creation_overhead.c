#include <pthread.h>
#include <stdio.h>
#include <time.h>

#define NUM_THREADS 10000

void* dummy_func(void* arg) {
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, dummy_func, NULL);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("One-to-One Creation: %f seconds for %d threads\n", time_taken, NUM_THREADS);
    printf("Average per thread: %f microseconds\n", (time_taken / NUM_THREADS) * 1e6);
    return 0;
}
