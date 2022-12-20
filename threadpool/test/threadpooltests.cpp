#include <gtest/gtest.h>
#include <threadpool.h>

#include <future>

class Add : public Task {
 public:
  Add(int a, int b) : m_a(a), m_b(b) {}

  void process() override { m_result.set_value(m_a + m_b); }

  int id() override { return 0; }

  auto get_future() { return m_result.get_future(); }

 private:
  int m_a;
  int m_b;
  std::promise<int> m_result;
};

TEST(threadpooltests, threadpool) {
  ThreadPool tp(4);

  Add add1(1, 31);
  Add add2(2, 31);
  Add add3(3, 31);
  Add add4(4, 31);
  Add add5(5, 31);
  Add add6(6, 31);

  tp.schedule_task(&add1);
  tp.schedule_task(&add2);
  tp.schedule_task(&add3);
  tp.schedule_task(&add4);
  tp.schedule_task(&add5);
  tp.schedule_task(&add6);

  auto result = add1.get_future();
  ASSERT_EQ(result.get(), 32);
  result = add2.get_future();
  ASSERT_EQ(result.get(), 33);
  result = add3.get_future();
  ASSERT_EQ(result.get(), 34);
  result = add4.get_future();
  ASSERT_EQ(result.get(), 35);
  result = add5.get_future();
  ASSERT_EQ(result.get(), 36);
  result = add6.get_future();
  ASSERT_EQ(result.get(), 37);

  tp.wait();
}
