#include <gtest/gtest.h>

#include "componentarray.h"
#include "constants.h"

struct Component {
  char m_name[32];
  int m_id = 0;
  float m_constant = 0;
};

TEST(componentarraytests, assign) {
  xen::ComponentArray<Component> arr;
  ASSERT_EQ(arr.assign(1), 0);
  ASSERT_EQ(arr.assign(xen::MAX_ENTITIES + 1), -1);
  ASSERT_EQ(arr.assign(1), -1);
}

TEST(componentarraytests, erase) {
  xen::ComponentArray<Component> arr;
  for (int i = 1; i < 129; i++) {
    ASSERT_EQ(arr.assign(i), 0);
  }

  for (int i = 1; i < 129; i++) {
    std::cout << i << "\n";
    ASSERT_EQ(arr.erase(i), 0);
  }
  ASSERT_EQ(arr.erase(10), -1);
}
