#include <gtest/gtest.h>

#include "vec.hpp"

TEST(vec_tests, vector_addition) {
    ge::vec3 v1{1, 2, 3};
    ge::vec3 v2{10, 10, 10};
    v1 += v2;
    ge::vec3 exp{11, 12, 13};
    ASSERT_EQ(v1, exp);
}