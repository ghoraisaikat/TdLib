# TDLib – A Simple User-Level Thread Library in C

TDLib is a lightweight user-level threading library implemented in C using the POSIX `ucontext` API. It provides cooperative/preemptive thread scheduling, context switching, thread lifecycle management, and a simple spinlock-based mutex implementation for synchronization.

The project also includes benchmarking programs that compare the library against POSIX (`pthread`) threads in terms of thread creation overhead, context switching, and blocking behavior.

---

## Features

* User-level thread creation
* Round-robin scheduler
* Timer-driven preemptive scheduling using `SIGALRM`
* Context switching with `ucontext`
* Thread lifecycle management
* Spinlock mutex using C11 atomics
* Benchmark suite comparing against `pthread`

---

## Project Structure

```text
.
├── Makefile
├── threads.c                 # Example program
├── include
│   ├── mut.h                 # Spinlock mutex implementation
│   └── uthread.h             # Public thread library API
├── src
│   └── uthread.c             # Thread library implementation
└── test
    ├── blocking.c            # pthread benchmark
    ├── blocking_u.c          # TDLib benchmark
    ├── context_switching.c
    ├── context_switching_u.c
    ├── creation_overhead.c
    └── creation_overhead_u.c
```

---

## Building

Compile the example program:

```bash
make
```

Run it:

```bash
make run
```

Clean generated binaries:

```bash
make clean
```

---

## Running Benchmarks

Build all benchmark programs:

```bash
make tests
```

Or compile individual benchmarks:

```bash
make creation
make blocking
make switching
```

---

## Thread API

The library exposes the following interface:

| Function              | Description                                |
| --------------------- | ------------------------------------------ |
| `thread_init()`       | Initialize the threading system            |
| `thread_create(func)` | Create a new user thread                   |
| `thread_start()`      | Start the scheduler                        |
| `thread_yield()`      | Yield execution to another runnable thread |
| `thread_exit()`       | Terminate the current thread               |
| `thread_cleanup()`    | Free resources after thread termination    |
| `print_threads()`     | Print the current thread table             |

---

## Scheduling

TDLib uses:

* Round-robin scheduling
* `SIGALRM` timer interrupts
* `setitimer()` for periodic preemption
* `swapcontext()` and `setcontext()` for context switching

Each thread has:

* Thread ID
* Execution context
* Private stack
* Execution state (`READY`, `RUNNING`, `WAITING`, `EXITED`)

---

## Synchronization

A lightweight spinlock mutex is implemented using C11 atomic flags.

Example:

```c
volatile mut_t mutex = MUTEX_INIT;

acquire(&mutex);
/* critical section */
release(&mutex);
```

---

## Benchmarks

The repository contains comparisons between TDLib and POSIX threads.

### Thread Creation

Measures the cost of creating and destroying large numbers of threads.

### Context Switching

Measures the overhead of repeatedly yielding execution between threads.

### Blocking

Demonstrates the difference between kernel threads and user-level threads when a blocking system call (such as `fgets()`) is invoked.

---

## Example

Running the demo program creates three user threads that execute concurrently:

* Thread 1 prints **"Bing!"**
* Thread 2 prints **"Bong!"**
* Thread 3 prints **"Boom!"**

The scheduler switches execution between them until all threads exit.

---

## Requirements

* GCC
* POSIX-compatible operating system (Linux/macOS)
* C11 compiler
* `ucontext` support

---

## Future Improvements

* Thread joining
* Sleeping/blocking queues
* Priority scheduling
* Condition variables
* Semaphores
* Dynamic scheduler policies
* Multi-core (M:N) scheduling

---
