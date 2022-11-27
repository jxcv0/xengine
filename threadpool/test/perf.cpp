#include <threadpool.h>
#include <iostream>

ThreadPool threadpool(std::thread::hardware_concurrency() - 1);
std::mutex os_mutex;

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

    for (i = 0; i < m_n; i++) {
      std::lock_guard lk(os_mutex);
      std::cout << f[i] << " ";
    }
  }

 private:
  int m_n;
};

int main() {
  Fib fib(10);
  threadpool.schedule_task(fib);
  return 0;
}
