#include <stdatomic.h>

#define MUTEX_INIT ATOMIC_FLAG_INIT
typedef atomic_flag mut_t;

// void acquire(mut_t* m)
#define acquire(m) \
	while (atomic_flag_test_and_set(m))

// void release(mut_t* m)
#define release(m) \
	atomic_flag_clear(m)
