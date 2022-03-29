#include <gtest/gtest.h>

#include "vec.hpp"

TEST(vec_tests, increment_vector) {
    ge::vec::vec3 v1{1, 2, 3};
    ge::vec::vec3 v2{10, 10, 10};
    v1 += v2;
    ge::vec::vec3 exp{11, 12, 13};
    ASSERT_EQ(v1, exp);
}

TEST(vec_tests, vector_addition) {
    ge::vec::vec3 v1{1, 2, 3};
    ge::vec::vec3 v2{10, 10, 10};
    auto sum = v1 + v2;
    ge::vec::vec3 exp{11, 12, 13};
    ASSERT_EQ(sum, exp);
}

TEST(vec_tests, product) {
    ge::vec::vec3 v1{1, 2, 3};
    ge::vec::vec3 v2{10, 10.5, 10};
    ge::vec::vec3 exp{10, 21, 30};
    ASSERT_EQ(v1 * v2, exp);
}