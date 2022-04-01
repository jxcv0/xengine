#include <gtest/gtest.h>

#include "alloc.hpp"

// i dont think this test works
TEST(alloc_tests, alloc_aligned) {
    auto ptr = reinterpret_cast<uintptr_t>(ge::alloc::alloc_aligned(100, 2));
    ASSERT_EQ(ptr % 2, 0);
}