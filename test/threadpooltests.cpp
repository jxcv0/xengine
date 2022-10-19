#include <gtest/gtest.h>
#include <threadpool.h>

TEST(threadpooltests, threadpool) {
  ThreadPool tp;
  auto result = tp.schedule_task([=](int a, int b){ return a + b; }, 5, 8);
}
