#include <gtest/gtest.h>

#include "archetype.h"
#include "system.h"

struct C1 {
  int i;
};

struct C2 {
  int i;
};

TEST(system_tests, execute) {
  auto arch = new xen::archetype<C1, C2>;
  (void)arch;
}
