#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* counter_thread(void* arg) {
    for (int i = 0; i < 10; i++) {
        printf("Counter: %d (I'm still running!)\n", i);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t t1;
    char buffer[10];

    pthread_create(&t1, NULL, counter_thread, NULL);

    printf("MAIN: Enter something to 'block' this thread: ");
    fgets(buffer, 10, stdin); // This blocks ONLY the main thread
    printf("MAIN: You entered: %s", buffer);

    pthread_join(t1, NULL);
    return 0;
}
