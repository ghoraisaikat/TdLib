#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <sched.h>

#define YIELDS 1000000

void* yielder(void* arg) {
    for (int i = 0; i < YIELDS; i++) {
        sched_yield();
    }
    return NULL;
}

int main() {
    pthread_t t1;
    struct timespec start, end;

    pthread_create(&t1, NULL, yielder, NULL);
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < YIELDS; i++) {
        sched_yield();
    }
    pthread_join(t1, NULL);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double total = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Total time for %d context switches: %f seconds\n", YIELDS * 2, total);
    return 0;
}
