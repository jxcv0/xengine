#include <cmath>
#include <future>
#include <threadpool.h>
#include <iostream>
#include <string>
#include <random>
#include <cmath>
#include <type_traits>

float rootrandstr() {
  float f1, f2;
  f1 = static_cast<float>(std::rand());
  f2 = static_cast<float>(std::rand());
  return std::sqrt(f1 + f2);
}

std::vector<std::future<float>> futures;

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cout << "usage:\n"
              << "  profile <number of threads> <number of tasks>\n";
    return -1;
  }
  auto n_threads = std::atoi(argv[1]);
  auto n_tasks = std::atoi(argv[2]);
  std::cout << "starting threadpool with " << n_threads << " threads\n";
  ThreadPool threadpool(n_threads);

  std::cout << "scheduling " << n_tasks << " tasks\n";
  for (auto i = 0; i < n_tasks; i++) {
    auto fut = threadpool.schedule_task(rootrandstr);
    futures.push_back(std::move(fut));
  }

  for (auto &fut : futures) {
    std::cout << "result: " << fut.get() << "\n";
  }

  std::cout << "done\n";

  return 0;
}
