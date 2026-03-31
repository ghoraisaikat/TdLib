#ifndef UTHREAD_H
#define UTHREAD_H

#define _XOPEN_SOURCE 600

#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_THREADS 10
#define STACK_SIZE SIGSTKSZ
#define QUANTUM 50000   // 50 ms

typedef enum {
    READY,
    RUNNING,
    WAITING,
    EXITED
} thread_state_t;

typedef struct {
    int id;
    ucontext_t ctx;
    void *stack;
    thread_state_t state;
} TCB;

/* Public API */
int thread_init(void);
int thread_create(void (*func)(void));
void thread_start(void);
int thread_yield(void);
void thread_exit(void);
void thread_cleanup(void);

/* Debug / utility */
void print_threads(void);

#endif
