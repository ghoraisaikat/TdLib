#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>

#define MAX_TASKS 10

#define READY 0
#define RUNNING 1
#define EXITED 2

typedef struct {
	int id;
	ucontext_t* ctx;
	void* stack;
	int state;

} TCB;

ucontext_t	*thread_init(void);
int		thread_create(void (*)(void));
int		thread_yield(void);
int		thread_cleanup(void);
