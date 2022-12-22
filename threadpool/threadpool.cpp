#include "threadpool.h"

#include <pthread.h>

#include <iostream>

/**
 * 	  1. Separate readonly (after initialization) from rw variables.
 * 	  2. Group rw variables that are used together into a struct so that
 * 	     memory locations are close together.
 * 	  3. Move rw variables that are shared between threads to thier own
 * 	     cacheline using padding.
 * 	  4. Variables shared by multiple threads but independantly should be
 * 	     moved to TLS.
 */

struct Task {
  task_fp m_fp;
  void *m_arg;
};

bool run = true;
pthread_t threads[NUM_THREADS];

// used together always
struct {
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
} sync;

// used together always
struct {
  Task tasks[TASK_BUFFER_SIZE];
  int num_tasks = 0;
} task_stack;

/**
 * ----------------------------------------------------------------------------
 */
static void *worker_loop([[maybe_unused]] void *arg) {
  for (;;) {
    pthread_mutex_lock(&sync.mutex);

    while (task_stack.num_tasks == 0 && run == true) {
      pthread_cond_wait(&sync.cond, &sync.mutex);
    }

    if (run == false && task_stack.num_tasks == 0) {
      break;
    }

    Task t = task_stack.tasks[--task_stack.num_tasks];
    pthread_mutex_unlock(&sync.mutex);
    pthread_cond_signal(&sync.cond);
    t.m_fp(t.m_arg);
  }
  pthread_mutex_unlock(&sync.mutex);
  pthread_cond_broadcast(&sync.cond);
  pthread_exit(nullptr);
  return nullptr;
}

/**
 * ----------------------------------------------------------------------------
 */
void threadpool::init() {
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], nullptr, worker_loop, nullptr);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void threadpool::stop() {
  pthread_mutex_lock(&sync.mutex);
  run = false;
  pthread_mutex_unlock(&sync.mutex);
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_cond_broadcast(&sync.cond);
    pthread_join(threads[i], nullptr);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void threadpool::schedule_task(task_fp fp, void *arg) {
  pthread_mutex_lock(&sync.mutex);
  while (task_stack.num_tasks == TASK_BUFFER_SIZE) {
    pthread_cond_wait(&sync.cond, &sync.mutex);
  }
  Task *t = &task_stack.tasks[task_stack.num_tasks++];
  t->m_fp = fp;
  t->m_arg = arg;
  pthread_mutex_unlock(&sync.mutex);
  pthread_cond_signal(&sync.cond);
}
