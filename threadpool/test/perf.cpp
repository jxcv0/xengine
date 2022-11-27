#include <threadpool.h>
#include <iostream>
#include <future>

ThreadPool threadpool(std::thread::hardware_concurrency() - 1);
std::mutex os_mutex;

class Timer {
public:
  using time_point = std::chrono::time_point<std::chrono::steady_clock>;

  Timer() {
    m_start = std::chrono::steady_clock::now();
  }

  ~Timer() {
    auto end = std::chrono::steady_clock::now();
    auto diff = end - m_start;
    std::cout << "time taken: " << std::chrono::duration_cast<std::chrono::microseconds>(end - m_start).count() << "\n";
  }

 private:
  time_point m_start;
};

class Fib : public Task {
 public:
  Fib(int n) : m_n(n) {}

  void process() override {
    int f[m_n];
    int i;
    f[0] = 0;
    f[1] = 1;
    for (i = 2; i < m_n; i++) {
      f[i] = f[i-1] + f[i-2];
    }

    /*
    for (i = 0; i < m_n; i++) {
      std::lock_guard lk(os_mutex);
      std::cout << f[i] << " ";
    }
    */
    m_complete.set_value();
  }

  auto get_future() {
    return m_complete.get_future();
  }

 private:
  std::promise<void> m_complete;
  int m_n;
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
  auto fut1 = f1.get_future();
  fut1.get();
  return 0;
}
