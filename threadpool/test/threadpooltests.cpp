#include <gtest/gtest.h>
#include <pthread.h>

#include "threadpool.h"

pthread_mutex_t lock;
int counter = 0;

void add_one() {
  pthread_mutex_lock(&lock);
  counter++;
  pthread_mutex_unlock(&lock);
}

TEST(threadpooltests, test) {
  threadpool::init();
  for (int i = 0; i < 1000; i++) {
    threadpool::schedule_task(add_one);
  }
  threadpool::stop();
  ASSERT_EQ(counter, 1000);
}

TEST(threadpooltests, wait) {
  threadpool::init();
  for (int i = 0; i < 1000; i++) {
    threadpool::schedule_task(add_one);
    std::cout << "loop\n";
  }
  threadpool::wait();
  std::cout << "wait\n";
  threadpool::stop();
  ASSERT_EQ(counter, 1000);
}
