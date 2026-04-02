#include "include/uthread.h"
#include "include/mut.h"

volatile mut_t mutex = MUTEX_INIT;

void func1(void) {
    for (int i = 0; i < 10; i++) {
    	acquire(&mutex);
        printf("Thread 1: Bing! (%d)\n", i);
        usleep(70000);
	release(&mutex);
    }
    printf("Thread 1 exiting...\n");
    thread_exit();
}

void func2(void) {
    for (int i = 0; i < 10; i++) {
        printf("Thread 2: Bong! (%d)\n", i);
        usleep(70000);
    }
    printf("Thread 2 exiting...\n");
    thread_exit();
}

void func3(void) {
    for (int i = 0; i < 10; i++) {
        printf("Thread 3: Boom! (%d)\n", i);
        usleep(70000);
    }
    printf("Thread 3 exiting...\n");
    thread_exit();
}

int main() {
    if (thread_init() == -1) {
        printf("Thread system initialization failed\n");
        return 1;
    }

    thread_create(func1);
    thread_create(func2);
    thread_create(func3);

    print_threads();

    printf("Starting thread scheduler...\n");
    thread_start();

    printf("All threads finished, back to main.\n");
    return 0;
}
