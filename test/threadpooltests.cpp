#include <gtest/gtest.h>
#include <threadpool.h>

int add(int a, int b) {
  return a + b;
}

TEST(threadpooltests, threadpool) {
  ThreadPool tp(1);
  auto result5 = tp.schedule_task(add, 5, 8);
  auto result6 = tp.schedule_task(add, 6, 8);
  auto result7 = tp.schedule_task(add, 7, 8);
  auto result8 = tp.schedule_task(add, 8, 8);
  auto result9 = tp.schedule_task(add, 9, 8);
  auto result10 = tp.schedule_task(add, 10, 8);

  int i = -1;
  i = result6.get();
  ASSERT_EQ(i, 14);
  i = result7.get();
  ASSERT_EQ(i, 15);
  i = result5.get();
  ASSERT_EQ(i, 13);
  i = result9.get();
  ASSERT_EQ(i, 17);
  i = result10.get();
  ASSERT_EQ(i, 18);
  i = result8.get();
  ASSERT_EQ(i, 16);
}
