#include <gtest/gtest.h>
#include <logallocator.h>

#include <functional>
#include <thread>
#include <type_traits>

TEST(logallocatortests, allocate) {
  LogAllocator<std::function<void(void)> > a;
  ASSERT_NO_THROW(auto i = a.allocate(10); a.deallocate(i, 10););
}

// manual test
TEST(logallocatortests, threads) {
  std::vector<std::thread> threads;
  for (auto i = 0; i < 3; i++) {
    threads.emplace_back(std::thread([]() {
      for (auto j = 0; j < 2; j++) {
        LogAllocator<float> a;
        auto i = a.allocate(1);
        a.deallocate(i, 1);
      }
    }));
  }
  for (auto &t : threads) {
    t.join();
  }
}
