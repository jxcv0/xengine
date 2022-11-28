#include <threadpool.h>

#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>

ThreadPool threadpool(std::thread::hardware_concurrency() - 1);
std::mutex shared_mutex;

class Timer {
 public:
  using time_point = std::chrono::time_point<std::chrono::steady_clock>;

  Timer() { m_start = std::chrono::steady_clock::now(); }

  ~Timer() {
    auto end = std::chrono::steady_clock::now();
    auto diff =
        std::chrono::duration_cast<std::chrono::microseconds>(end - m_start)
            .count();
    std::cout << "time taken: " << diff << " us\n";
  }

 private:
  time_point m_start;
};

class Fib : public Task {
 public:
  Fib(int n) : m_count(n), m_result(n) {}
  virtual ~Fib() = default;

  void process() override {
    int i;
    m_result[0] = 0;
    m_result[1] = 1;
    for (i = 2; i < m_count; i++) {
      m_result[i] = m_result[i - 1] + m_result[i - 2];
    }
  }

 private:
  std::vector<int> m_result;
  int m_count;
};

int main() {
  Timer t;
  Fib f1(10);
  threadpool.schedule_task(f1);
  Fib f2(10);
  threadpool.schedule_task(f2);
  Fib f3(10);
  threadpool.schedule_task(f3);
  Fib f4(10);
  threadpool.schedule_task(f4);
  Fib f5(10);
  threadpool.schedule_task(f5);
  Fib f6(10);
  threadpool.schedule_task(f6);
  return 0;
}
