#include <gtest/gtest.h>

#include "ecs.h"

struct A {
  static const int id = 1;
  int i = 0;
};

struct B {
  static const int id = 2;
  int i = 13;
};

struct C {
  static const int id = 3;
  int i = 120;
};

TEST(ecstests, test) {
  ECS ecs;
  (void)ecs;
}
