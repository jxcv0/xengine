#include <gtest/gtest.h>
#include <threadpool.h>

class Add : public Task {
 public:
  Add(int a, int b) : m_a(a), m_b(b), m_done(false) {}

  void process() override {
    m_result = m_a + m_b;
    m_done.store(true);
  }

  int id() override {
    return 0;
  }

  auto get_result() {
    while (!m_done) {
    }  // yuck
    return m_result;
  }

 private:
  int m_a;
  int m_b;
  int m_result;
  std::atomic<bool> m_done;
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
  tp.schedule_task(&add1);
  tp.schedule_task(&add2);
  tp.schedule_task(&add3);
  tp.schedule_task(&add4);
  tp.schedule_task(&add5);
  tp.schedule_task(&add6);

  int result;
  result = add1.get_result();
  ASSERT_EQ(result, 32);
  result = add2.get_result();
  ASSERT_EQ(result, 33);
  result = add3.get_result();
  ASSERT_EQ(result, 34);
  result = add4.get_result();
  ASSERT_EQ(result, 35);
  result = add5.get_result();
  ASSERT_EQ(result, 36);
  result = add6.get_result();
  ASSERT_EQ(result, 37);
}
