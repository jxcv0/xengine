#include <gtest/gtest.h>

#include "alloc.hpp"

#include <random>

// i dont think this test works
TEST(alloc_tests, alloc_aligned) {  
    int *ptr;
    int *aligned_ptr = ge::alloc::align_ptr(ptr, 16);
    ASSERT_EQ(reinterpret_cast<uintptr_t>(aligned_ptr) % alignof(aligned_ptr), 0);
    ASSERT_EQ((uintptr_t)aligned_ptr % 16, 0);
}