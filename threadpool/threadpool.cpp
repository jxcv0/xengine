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

pthread_t threads[NUM_THREADS];

// used together always
struct {
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
  task_fp tasks[TASK_BUFFER_SIZE];
  int num_tasks = 0;
  bool run = true;
} tp;

/**
 * ----------------------------------------------------------------------------
 */
static void *worker_loop([[maybe_unused]] void *arg) {
  for (;;) {
    pthread_mutex_lock(&tp.mutex);

    while (tp.num_tasks == 0 && tp.run == true) {
      pthread_cond_wait(&tp.cond, &tp.mutex);
    }

    if (tp.run == false && tp.num_tasks == 0) {
      break;
    }

    task_fp t = tp.tasks[--tp.num_tasks];
    pthread_mutex_unlock(&tp.mutex);
    pthread_cond_signal(&tp.cond);
    t();
  }
  pthread_mutex_unlock(&tp.mutex);
  pthread_cond_broadcast(&tp.cond);
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
  pthread_mutex_lock(&tp.mutex);
  tp.run = false;
  pthread_mutex_unlock(&tp.mutex);
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_cond_broadcast(&tp.cond);
    pthread_join(threads[i], nullptr);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void threadpool::wait() {
  pthread_mutex_lock(&tp.mutex);
  while (tp.num_tasks != 0 && tp.run == true) {
    pthread_cond_wait(&tp.cond, &tp.mutex);
  }
  pthread_mutex_unlock(&tp.mutex);
}

/**
 * ----------------------------------------------------------------------------
 */
void threadpool::schedule_task(task_fp fp) {
  pthread_mutex_lock(&tp.mutex);
  while (tp.num_tasks == TASK_BUFFER_SIZE && tp.run == true) {
    pthread_cond_wait(&tp.cond, &tp.mutex);
  }

  if (tp.run) {
    tp.tasks[tp.num_tasks++] = fp;
  }

  pthread_mutex_unlock(&tp.mutex);
  pthread_cond_signal(&tp.cond);
}
