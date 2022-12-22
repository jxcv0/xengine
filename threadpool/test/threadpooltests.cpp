#include <gtest/gtest.h>
#include <pthread.h>

#include "threadpool.h"

pthread_mutex_t lock;
int counter = 0;

void add_one(void *arg) {
  pthread_mutex_lock(&lock);
  int *c = static_cast<int *>(arg);
  *c = *c + 1;
  pthread_mutex_unlock(&lock);
}

TEST(threadpooltests, test) {
  threadpool::init();
  for (int i = 0; i < 1000; i++) {
    threadpool::schedule_task(add_one, &counter);
  }
  threadpool::stop();
  ASSERT_EQ(counter, 1000);
}
