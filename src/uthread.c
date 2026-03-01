#include "uthread.h"

// Increment this to assign a TID
static tids = 0;
static TCB* RUN[MAX_TASKS];
static TCB* running_thread;

ucontext_t*
thread_init(void)
{
	ucontext_t* ctx_main = (ucontext_t *) malloc(sizeof ucontext_t);
	if (getcontext(ctx_main) == -1)
		return NULL;
	for (int i = 0; i < MAX_TASKS; i++)
		RUN[i] = NULL; // Careful
	return ctx_main;
}

int
thread_create(void (*f)(void))
{
	if (tids == MAX_TASKS)
		return -1;

	TCB* thread = (TCB *) calloc(1, sizeof TCB);
	thread->id = tids++;
	thread->ctx  = (ucontext_t *) malloc(sizeof ucontext_t);
	thread->stack = calloc(1, 4096);
	thread->state = READY;
	
	thread->ctx->uc_stack.ss_sp = thread->stack;
	thread->ctx->uc_stack.ss_size = 4096;
	thread->ctx->uc_link = NULL;
	makecontext(thread->ctx, f, 0);

	RUN[thread->id] = thread;
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
