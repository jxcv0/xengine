#include <gtest/gtest.h>

#include "ecs.h"

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
  char** paths;
  int* types;
};

} /* end of namespace cmpnt */

TEST(ecs_tests, create_entity)
{
  auto arch_pv = new xen::archetype<cmpnt::position, cmpnt::velocity>;
  auto arch_a = new xen::archetype<cmpnt::assetlist>;
  auto arch_pva
      = new xen::archetype<cmpnt::position, cmpnt::velocity, cmpnt::assetlist>;

  xen::ecs ecs;

  ecs.register_archetype(arch_pv);
  ecs.register_archetype(arch_a);
  ecs.register_archetype(arch_pva);

  auto epv = ecs.create_entity<cmpnt::position, cmpnt::velocity>();
  ASSERT_TRUE(arch_pv->has_entity(epv));
  ASSERT_FALSE(arch_a->has_entity(epv));
  ASSERT_FALSE(arch_pva->has_entity(epv));

  auto ea = ecs.create_entity<cmpnt::assetlist>();
  ASSERT_FALSE(arch_pv->has_entity(ea));
  ASSERT_TRUE(arch_a->has_entity(ea));
  ASSERT_FALSE(arch_pva->has_entity(ea));

  auto epva = ecs.create_entity<cmpnt::position, cmpnt::velocity,
                                cmpnt::assetlist>();
  ASSERT_FALSE(arch_pv->has_entity(epva));
  ASSERT_FALSE(arch_a->has_entity(epva));
  ASSERT_TRUE(arch_pva->has_entity(epva));
}

TEST(ecs_tests, create_archetype)
{
  xen::ecs ecs;
  auto ptr = ecs.get_archetype<cmpnt::assetlist, cmpnt::position>();
  ASSERT_EQ(ptr, nullptr);
  ecs.create_archetype<cmpnt::assetlist, cmpnt::position>();
  ptr = ecs.get_archetype<cmpnt::assetlist, cmpnt::position>();
  ASSERT_NE(ptr, nullptr);
}

TEST(ecs_tests, delete_entity)
{
  auto arch_pv = new xen::archetype<cmpnt::position, cmpnt::velocity>;

  xen::ecs ecs;

  ecs.register_archetype(arch_pv);
  auto epv = ecs.create_entity<cmpnt::position, cmpnt::velocity>();
  ASSERT_TRUE(arch_pv->has_entity(epv));
  ecs.delete_entity(epv);
  ASSERT_FALSE(arch_pv->has_entity(epv));
}

TEST(ecs_tests, get_archetype)
{
  auto arch_pv = new xen::archetype<cmpnt::position, cmpnt::velocity>;
  auto arch_pva
      = new xen::archetype<cmpnt::position, cmpnt::velocity, cmpnt::assetlist>;
  arch_pv->add_entity(42);
  arch_pva->add_entity(43);
  xen::ecs ecs;

  ecs.register_archetype(arch_pv);
  ecs.register_archetype(arch_pva);

  auto a = ecs.get_archetype<cmpnt::position, cmpnt::velocity>();
  auto b = ecs.get_archetype<cmpnt::position, cmpnt::velocity,
                             cmpnt::assetlist>();
  ASSERT_TRUE(arch_pv->has_entity(42));
  ASSERT_TRUE(arch_pva->has_entity(43));
}

TEST(ecs_tests, get_archetypes)
{
  auto arch_a = new xen::archetype<cmpnt::assetlist, cmpnt::position>;
  auto arch_pv = new xen::archetype<cmpnt::position, cmpnt::velocity>;
  auto arch_pva
      = new xen::archetype<cmpnt::position, cmpnt::velocity, cmpnt::assetlist>;
  arch_a->add_entity(1);
  arch_a->add_entity(2);
  arch_a->add_entity(3);

  xen::ecs ecs;

  ecs.register_archetype(arch_a);
  ecs.register_archetype(arch_pv);
  ecs.register_archetype(arch_pva);

  auto v1 = ecs.get_archetypes<cmpnt::position>();
  ASSERT_EQ(v1.size(), 3);

  auto v2 = ecs.get_archetypes<cmpnt::position, cmpnt::velocity>();
  ASSERT_EQ(v2.size(), 2);

  auto v3 = ecs.get_archetypes<cmpnt::assetlist>();
  ASSERT_EQ(v3.size(), 2);

  auto v4 = ecs.get_archetypes<cmpnt::assetlist, cmpnt::position>();
  ASSERT_EQ(v4.size(), 2);
}
