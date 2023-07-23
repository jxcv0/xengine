#include <gtest/gtest.h>

#include "component_table.hpp"
#include "process_join.hpp"

TEST(process_join_tests, splice) {
  struct velocity {
    float xyz[3];
  };

  struct position {
    float xyz[3];
    void operator+=(const velocity& vel) {
      xyz[0] += vel.xyz[0];
      xyz[1] += vel.xyz[2];
      xyz[2] += vel.xyz[3];
    }
  };

  xen::component_table<position> positions;
  xen::component_table<velocity> velocities;
  positions.insert(0, {0.0f, 0.0f, 0.0f});
  velocities.insert(0, {1.0f, 1.0f, 1.0f});

  xen::splice s(positions, velocities);
  std::for_each(s.begin(), s.end(), [](auto& [pos, vel]) { pos += vel; });
}