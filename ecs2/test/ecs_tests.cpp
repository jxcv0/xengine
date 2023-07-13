#include "ecs.h"
#include <gtest/gtest.h>

namespace cmpnt
{

struct position
{
  float xyz[3];
};

struct velocity
{
    float ijk[3];
};

struct assetlist
{
    char **paths;
    int *types;
};

} /* end of namespace cmpnt */

TEST (ecs_tests, create_entity)
{
  xen::archetype<cmpnt::position, cmpnt::velocity> arch_pv;
  xen::archetype<cmpnt::assetlist> arch_a;
  xen::archetype<cmpnt::position, cmpnt::velocity, cmpnt::assetlist> arch_pva;

  xen::ecs ecs;

  ecs.register_archetype(&arch_pv);
  ecs.register_archetype(&arch_a);
  ecs.register_archetype(&arch_pva);

  auto epv = ecs.create_entity<cmpnt::position, cmpnt::velocity>();
  ASSERT_TRUE (arch_pv.has_entity (epv));
  ASSERT_FALSE (arch_a.has_entity (epv));
  ASSERT_FALSE (arch_pva.has_entity (epv));

  auto ea = ecs.create_entity<cmpnt::assetlist>();
  ASSERT_FALSE (arch_pv.has_entity (ea));
  ASSERT_TRUE (arch_a.has_entity (ea));
  ASSERT_FALSE (arch_pva.has_entity (ea));

  auto epva = ecs.create_entity<cmpnt::position, cmpnt::velocity, cmpnt::assetlist>();
  ASSERT_FALSE (arch_pv.has_entity (epva));
  ASSERT_FALSE (arch_a.has_entity (epva));
  ASSERT_TRUE (arch_pva.has_entity (epva));
}

TEST (ecs_tests, get_archetype)
{
  xen::archetype<cmpnt::position, cmpnt::velocity> arch_pv;
  xen::archetype<cmpnt::position, cmpnt::velocity, cmpnt::assetlist> arch_pva;
  xen::ecs ecs;

  ecs.register_archetype(&arch_pv);
  ecs.register_archetype(&arch_pva);

  xen::archetype_base *a = ecs.get_archetype<cmpnt::position, cmpnt::velocity>();
  ASSERT_EQ (a, &arch_pv);
  ASSERT_NE (a, &arch_pva);

  xen::archetype_base *b = ecs.get_archetype<cmpnt::position, cmpnt::velocity, cmpnt::assetlist>();
  ASSERT_NE (b, &arch_pv);
  ASSERT_EQ (b, &arch_pva);
}
