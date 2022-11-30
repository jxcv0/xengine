#include <threadpool.h>

#include <atomic>
#include <clocale>
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>

ThreadPool threadpool(std::thread::hardware_concurrency() - 1);

std::mutex counter_mutex;
std::condition_variable counter_cv;
std::atomic_int counter = 0;

class Timer {
 public:
  using time_point = std::chrono::time_point<std::chrono::steady_clock>;

  Timer() { m_start = std::chrono::steady_clock::now(); }

  ~Timer() {
    auto end = std::chrono::steady_clock::now();
    auto diff =
        std::chrono::duration_cast<std::chrono::microseconds>(end - m_start)
            .count();
    std::cout << diff << "us\n";
  }

 private:
  time_point m_start;
};

class Fib : public Task {
 public:
  Fib(int n) : m_count(n) {}
  virtual ~Fib() = default;

  void process() override {
    unsigned int f[m_count];
    unsigned int i;
    f[0] = 0;
    f[1] = 1;
    for (i = 2; i < m_count; i++) {
      f[i] = f[i - 1] + f[i - 2];
    }
    std::lock_guard lk(counter_mutex);
    std::cout << "fib calculated: ";
    for (i = 0; i < m_count; i++) {
      std::cout << f[i] << " ";
    }
    std::cout << "\n";
    counter++;
    counter_cv.notify_one();
  }

 private:
  unsigned int m_count;
};

int main() {
  std::cout << sizeof(ThreadPool) << "\n\n";
  counter = 0;
  Timer t;
  Fib f1(9);
  Fib f2(14);
  Fib f3(13);
  Fib f4(22);
  Fib f5(14);
  Fib f6(10);
  threadpool.schedule_task(&f1);
  threadpool.schedule_task(&f2);
  threadpool.schedule_task(&f3);
  threadpool.schedule_task(&f4);
  threadpool.schedule_task(&f5);
  threadpool.schedule_task(&f6);
  threadpool.schedule_task(&f3);
  threadpool.schedule_task(&f4);
  threadpool.schedule_task(&f5);
  threadpool.schedule_task(&f6);
  threadpool.schedule_task(&f5);
  threadpool.schedule_task(&f6);
  threadpool.schedule_task(&f1);
  threadpool.schedule_task(&f2);
  threadpool.schedule_task(&f3);
  threadpool.schedule_task(&f4);
  threadpool.schedule_task(&f5);
  threadpool.schedule_task(&f6);
  threadpool.schedule_task(&f3);
  threadpool.schedule_task(&f4);
  threadpool.schedule_task(&f5);

  // wait for threads to finish before destroying tasks.
  std::unique_lock lk(counter_mutex);
  counter_cv.wait(lk, [&] { return counter == 21; });
  std::cout << "time taken to complete tasks: ";
  return 0;
}

// first iteration 60 us average
//
