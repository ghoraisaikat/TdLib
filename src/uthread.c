#include "uthread.h"

// Increment this to assign a TID
static tids = 0;
// Declare a run queue, the running thread, and the TCB for a cleanup handler
static TASKS = 0;
static TCB* RUN[MAX_TASKS];
static TCB* running_thread;
static TCB* cleanup_handler;

static ucontext_t* ctx_main;

int
thread_init(void)
{
	// Save a snapshot of the current process
	// Must be called before doing anything with the threads;
	ucontext_t* main = (ucontext_t *) calloc(1, sizeof ucontext_t);
	if (getcontext(main) == -1)
		return -1;
	// Initialize the cleanup handler
	ucontext_t* 	ctx_cleanup 	= (ucontext_t *) calloc(1, sizeof ucontext_t);
	void* 		st_cleanup 	= calloc(1, SIGSTKSZ);
	int 		state_cleanup	= READY;
	if (getcontext(ctx_cleanup) == -1)
		return -1;

	ctx_cleanup->uc_stack.ss_sp 	= st_cleanup;
	ctx_cleanup->uc_stack.ss_size	= SIGSTKSZ;
	// The cleanup handler should never return
	ctx_cleanup->uc_link.uc_link	= NULL;

	TCB* tcb_cleanup 	= (TCB *) calloc(1, sizeof TCB);
	tcb_cleanup->id 	= -1;
	tcb_cleanup->stack	= st_cleanup;
	tcb_cleanup->ctx	= ctx_cleanup;
	tcb_cleanup->state	= state_cleanup;
	makecontext(tcb_cleanup->ctx, thread_cleanup, 0);	
	
	// Initialize an empty run queue
	// Timer is not implemented yet.
	for (int i = 0; i < MAX_TASKS; i++)
		RUN[i] = NULL;

	// Set global static variable to allocated variable
	ctx_main = main;
	return 0;
}

int
thread_cleanup(void)
{
	sigset_t signals;
	sigemptyset(&signals);

	sigaddset(&signals, SIGALRM);
	sigaddset(&signals, SIGINT);
	
	sigprocmask(SIG_BLOCK, &signals, NULL);
	
	int tid = running_thread->id;
	free(running_thread->ctx);
	free(running_thread->stack);
	free(running_thread);
	
	RUN[tid] = NULL;

	bool fd = false;
	int current = (tid + 1) % MAX_TASKS;
	TASKS--;
	// Find the next runnable task;
	for (int i = 0; i < TASKS; i++) {
		if (RUN[current]) {
			fd = true;
			break;
		}
		current = (current + 1) % MAX_TASKS;
	}
	if (fd == false) setcontext(ctx_main);
	else {
		running_thread = RUN[current];
		setcontext(RUN[current]->ctx);
	}

	return 0;
}

int
thread_create(void (*f)(void))
{
	// Check if RUN queue is full
	int i = 0;
	for (; i < MAX_TASKS; i++) {
		if (RUN[i] == NULL) {
			break;
		}
	}
	if (i == MAX_TASKS)
		return -1;

	// Allocate space for new thread
	TCB* thread = (TCB *) calloc(1, sizeof TCB);
	thread->id = i;
	thread->ctx  = (ucontext_t *) malloc(sizeof ucontext_t);
	thread->stack = calloc(1, SIGSTKSZ);
	thread->state = READY;
	if (getcontext(thread->ctx) == -1)
		return -1;

	thread->ctx->uc_stack.ss_sp = thread->stack;
	thread->ctx->uc_stack.ss_size = SIGSTKSZ;
	thread->ctx->uc_link = cleanup_handler->ctx;
	
	makecontext(thread->ctx, (void (*)(void))f, 0);

	RUN[thread->id] = thread;
	TASKS++;
	return thread->id;

}

int
thread_yield(void)
{
	sigset_t signal_set;
	sigemptyset(&signal_set);

	sigaddset(&signal_set, SIGALRM);
	sigaddset(&signal_set, SIGINT);

	sigprocmask(SIG_BLOCK, &signal_set, NULL);
	// CS
	sigprocmask(SIG_UNBLOCK, &signal_set, NULL);
}
