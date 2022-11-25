#include <gtest/gtest.h>
#include <logallocator.h>

#include <functional>

TEST(logallocatortests, allocate) {
    LogAllocator<std::function<void(void)>> a;
    auto *i = a.allocate(10);
}
