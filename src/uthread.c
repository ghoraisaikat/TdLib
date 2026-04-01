#include "../include/uthread.h"


static TCB *threads[MAX_THREADS];
static TCB *current_thread = NULL;

static ucontext_t main_ctx;
static ucontext_t cleanup_ctx;
static void *cleanup_stack = NULL;

static int thread_count = 0;

static int find_next_thread() {
    if (current_thread == NULL) return -1;

    int start = current_thread->id;
    int next = (start + 1) % MAX_THREADS;

    for (int i = 0; i < MAX_THREADS; i++) {
        if (threads[next] != NULL && threads[next]->state == READY) {
            return next;
        }
        next = (next + 1) % MAX_THREADS;
    }

    return -1;
}

static void scheduler(int signum) {
    (void) signum;
    thread_yield();
}

static void start_timer() {
    struct itimerval timer;

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = QUANTUM;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = QUANTUM;

    setitimer(ITIMER_REAL, &timer, NULL);
}

int thread_init(void) {
    for (int i = 0; i < MAX_THREADS; i++) {
        threads[i] = NULL;
    }

    if (getcontext(&main_ctx) == -1) {
        perror("getcontext(main)");
        return -1;
    }

    cleanup_stack = malloc(STACK_SIZE);
    if (!cleanup_stack) {
        perror("malloc(cleanup_stack)");
        return -1;
    }

    if (getcontext(&cleanup_ctx) == -1) {
        perror("getcontext(cleanup)");
        return -1;
    }

    cleanup_ctx.uc_stack.ss_sp = cleanup_stack;
    cleanup_ctx.uc_stack.ss_size = STACK_SIZE;
    cleanup_ctx.uc_link = NULL;

    makecontext(&cleanup_ctx, thread_cleanup, 0);

    signal(SIGALRM, scheduler);
    start_timer();

    return 0;
}

int thread_create(void (*func)(void)) {
    int tid = -1;

    for (int i = 0; i < MAX_THREADS; i++) {
        if (threads[i] == NULL) {
            tid = i;
            break;
        }
    }

    if (tid == -1) {
        printf("No free thread slots available\n");
        return -1;
    }

    TCB *t = (TCB *) malloc(sizeof(TCB));
    if (!t) {
        perror("malloc(TCB)");
        return -1;
    }

    t->stack = malloc(STACK_SIZE);
    if (!t->stack) {
        perror("malloc(stack)");
        free(t);
        return -1;
    }

    t->id = tid;
    t->state = READY;

    if (getcontext(&t->ctx) == -1) {
        perror("getcontext(thread)");
        free(t->stack);
        free(t);
        return -1;
    }

    t->ctx.uc_stack.ss_sp = t->stack;
    t->ctx.uc_stack.ss_size = STACK_SIZE;
    t->ctx.uc_link = &cleanup_ctx;

    makecontext(&t->ctx, func, 0);

    threads[tid] = t;
    thread_count++;

    return tid;
}

void thread_start(void) {
    for (int i = 0; i < MAX_THREADS; i++) {
        if (threads[i] != NULL && threads[i]->state == READY) {
            current_thread = threads[i];
            current_thread->state = RUNNING;
            swapcontext(&main_ctx, &current_thread->ctx);
            return;
        }
    }
}

int thread_yield(void) {
    if (current_thread == NULL) return -1;

    int next_id = find_next_thread();

    if (next_id == -1) {
        return 0;     }

    TCB *prev = current_thread;
    TCB *next = threads[next_id];

    prev->state = READY;
    next->state = RUNNING;
    current_thread = next;

    swapcontext(&prev->ctx, &next->ctx);
    return 0;
}

void thread_exit(void) {
    if (current_thread == NULL) return;

    current_thread->state = EXITED;
    setcontext(&cleanup_ctx);
}

void thread_cleanup(void) {
    if (current_thread != NULL) {
        int tid = current_thread->id;

        free(current_thread->stack);
        free(current_thread);
        threads[tid] = NULL;
        current_thread = NULL;
        thread_count--;
    }

    if (thread_count == 0) {
        setcontext(&main_ctx);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        if (threads[i] != NULL && threads[i]->state == READY) {
            current_thread = threads[i];
            current_thread->state = RUNNING;
            setcontext(&current_thread->ctx);
        }
    }

    setcontext(&main_ctx);
}

void print_threads(void) {
    printf("------ THREAD TABLE ------\n");
    for (int i = 0; i < MAX_THREADS; i++) {
        if (threads[i] != NULL) {
            printf("TID: %d | State: %d\n", threads[i]->id, threads[i]->state);
        }
    }
    printf("--------------------------\n");
}
