#include "entity_mgr.h"
#include <gtest/gtest.h>

struct vec3
{
  float m_elem[3];
};

struct mat4
{
  float m_elem[4][4];
};

struct position
{
  float xyz[3];
};

TEST (entity_mgr_tests, create_and_delete_entities)
{
  xen::entity_mgr mgr;
  std::uint64_t e1 = mgr.create_entity ();
  ASSERT_EQ (e1, 0);
  ASSERT_EQ (mgr.num_entities (), 1);

  mgr.delete_entity (e1);
  ASSERT_EQ (mgr.num_entities (), 0);
  std::uint64_t e2 = mgr.create_entity ();
  ASSERT_EQ (mgr.num_entities (), 1);
  ASSERT_EQ (e1, e2); /* ID should be reused */
}

TEST (entity_mgr_tests, add_remove_components)
{
  xen::entity_mgr mgr;
  std::uint64_t e1 = mgr.create_entity ();
  ASSERT_FALSE (mgr.has_component<vec3> (e1));
  mgr.add_component<vec3> (e1);
  ASSERT_TRUE (mgr.has_component<vec3> (e1));
  mgr.delete_entity (e1);
  ASSERT_THROW (mgr.has_component<vec3> (e1), std::out_of_range);

  std::uint64_t e2 = mgr.create_entity ();
  mgr.add_components<mat4, vec3, position> (e2);
  ASSERT_TRUE (mgr.has_component<mat4> (e1));
  ASSERT_TRUE (mgr.has_component<vec3> (e1));
  ASSERT_TRUE (mgr.has_component<position> (e1));
}

TEST (entity_mgr_tests, has_components)
{
  xen::entity_mgr mgr;
  std::uint64_t e1 = mgr.create_entity ();
  ASSERT_FALSE (mgr.has_component<vec3> (e1));
  ASSERT_FALSE (mgr.has_component<mat4> (e1));
  mgr.add_component<vec3> (e1);
  mgr.add_component<mat4> (e1);
  ASSERT_TRUE (mgr.has_component<vec3> (e1));
  ASSERT_TRUE (mgr.has_component<vec3> (e1));
  bool b = mgr.has_components<vec3, mat4> (e1);
  ASSERT_TRUE (b);
}

TEST (entity_mgr_tests, count_archetype)
{
  xen::entity_mgr mgr;
  std::uint64_t e1 = mgr.create_entity ();
  std::uint64_t e2 = mgr.create_entity ();

  mgr.add_component<vec3> (e1);

  std::size_t res = mgr.count_archetype<vec3> ();
  ASSERT_EQ (res, 1);
  res = mgr.count_archetype<vec3, mat4> ();
  ASSERT_EQ (res, 0);
  res = mgr.count_archetype<vec3, mat4, position> ();
  ASSERT_EQ (res, 0);

  mgr.add_component<mat4> (e2);

  res = mgr.count_archetype<vec3> ();
  ASSERT_EQ (res, 1);
  res = mgr.count_archetype<vec3, mat4> ();
  ASSERT_EQ (res, 0);
  res = mgr.count_archetype<vec3, mat4, position> ();
  ASSERT_EQ (res, 0);
  res = mgr.count_archetype<mat4> ();
  ASSERT_EQ (res, 1);

  mgr.add_component<mat4> (e1);

  res = mgr.count_archetype<vec3> ();
  ASSERT_EQ (res, 1);
  res = mgr.count_archetype<vec3, mat4> ();
  ASSERT_EQ (res, 1);
  res = mgr.count_archetype<vec3, mat4, position> ();
  ASSERT_EQ (res, 0);

  mgr.add_component<position> (e1);

  res = mgr.count_archetype<vec3> ();
  ASSERT_EQ (res, 1);
  res = mgr.count_archetype<vec3, mat4> ();
  ASSERT_EQ (res, 1);
  res = mgr.count_archetype<vec3, mat4, position> ();
  ASSERT_EQ (res, 1);
}

TEST (entity_mgr_tests, get_archetype)
{
  xen::entity_mgr mgr;
  std::uint64_t e1 = mgr.create_entity ();
  std::uint64_t e2 = mgr.create_entity ();
  std::uint64_t e3 = mgr.create_entity ();

  mgr.add_components<mat4, vec3>(e1);
  mgr.add_components<vec3, position>(e2);
  mgr.add_components<mat4, vec3, position>(e3);

  uint64_t *eids1 = new uint64_t[mgr.count_archetype<mat4, vec3>()];
  mgr.get_archetype<mat4, vec3>(eids1);
  ASSERT_EQ (eids1[0], e3); /* order doesnt matter */
  ASSERT_EQ (eids1[1], e1);

  uint64_t *eids2 = new uint64_t[mgr.count_archetype<position>()];
  mgr.get_archetype<position>(eids2);
  ASSERT_EQ (eids2[0], e3); /* order doesnt matter */
  ASSERT_EQ (eids2[1], e2);

  uint64_t *eids3 = new uint64_t[mgr.count_archetype<mat4, vec3, position>()];
  mgr.get_archetype<position>(eids3);
  ASSERT_EQ (eids3[0], e3); /* order doesnt matter */
}
