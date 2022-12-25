#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#define TASK_BUFFER_SIZE 32
#define NUM_THREADS 6

typedef void (*task_fp)(void);

namespace threadpool {
/**
 * @breif Start worker threads.
 */
void init();

/**
 * @brief Join worker threads.
 */
void stop();

/**
 * @brief Block untill current tasks are complete.
 */
void wait();

/**
 * @breif Schedule a task to be completed in another thread.
 */
void schedule_task(task_fp fp);

}  // namespace threadpool

#endif  // THREADPOOL_H_
