#include <gtest/gtest.h>
#include <string.h>
#include "ecs.h"

struct some_component
{
  eid_t entity;
  int a;
  float b;
};

TEST (ecs_tests, component_castable_to_eid)
{
  struct some_component c;
  c.entity = 12345;
  eid_t *eid = (eid_t *) &c;
  ASSERT_EQ (*eid, 12345);
}

TEST (ecs_tests, create_component_array)
{
  struct ecs ecs;
  memset (&ecs, 0, sizeof(ecs));
  cid_t new_array =
      create_component_array (&ecs, sizeof (struct some_component), 20);
  ASSERT_EQ (new_array, 0);
  ASSERT_EQ (ecs.num_component_types, 1);
  ASSERT_NE (ecs.arrays[0].buf, nullptr);
  ASSERT_EQ (ecs.arrays[0].bufsize, sizeof (struct some_component) * 20);
  ASSERT_EQ (ecs.arrays[0].num_components, 0);
  ASSERT_EQ (ecs.arrays[0].stride, sizeof (struct some_component));
}

TEST (ecs_tests, assign_component)
{
  struct ecs ecs;
  memset (&ecs, 0, sizeof(ecs));
  eid_t e1 = create_entity(&ecs);
  cid_t new_array =
      create_component_array (&ecs, sizeof (struct some_component), 20);

  ASSERT_EQ (assign_component (&ecs, e1, new_array), 0);
  ASSERT_EQ (ecs.arrays[new_array].num_components, 1);
  ASSERT_EQ (ecs.arrays[new_array].map[0].entity, e1);
  ASSERT_EQ (ecs.arrays[new_array].map[0].offset, 0);

  eid_t e2 = create_entity(&ecs);
  ASSERT_EQ (assign_component (&ecs, e2, new_array), 0);
  ASSERT_EQ (ecs.arrays[new_array].num_components, 2);
  ASSERT_EQ (ecs.arrays[new_array].map[1].entity, e2);
  ASSERT_EQ (ecs.arrays[new_array].map[1].offset, sizeof (struct some_component));

  eid_t e3 = create_entity(&ecs);
  ASSERT_EQ (assign_component (&ecs, e3, new_array), 0);
  ASSERT_EQ (ecs.arrays[new_array].num_components, 3);
  ASSERT_EQ (ecs.arrays[new_array].map[2].entity, e3);
  ASSERT_EQ (ecs.arrays[new_array].map[2].offset, sizeof (struct some_component) * 2);
}

TEST (ecs_tests, unassign_component)
{
  struct ecs ecs;
  memset (&ecs, 0, sizeof(ecs));
  cid_t new_array =
      create_component_array (&ecs, sizeof (struct some_component), 20);

  eid_t e1 = create_entity(&ecs);
  ASSERT_EQ (assign_component (&ecs, e1, new_array), 0);
  ASSERT_EQ (ecs.arrays[new_array].num_components, 1);
  ASSERT_EQ (ecs.arrays[new_array].map[0].entity, e1);
  ASSERT_EQ (ecs.arrays[new_array].map[0].offset, 0);

  eid_t e2 = create_entity(&ecs);
  ASSERT_EQ (assign_component (&ecs, e2, new_array), 0);
  ASSERT_EQ (ecs.arrays[new_array].num_components, 2);
  ASSERT_EQ (ecs.arrays[new_array].map[1].entity, e2);
  ASSERT_EQ (ecs.arrays[new_array].map[1].offset, sizeof (struct some_component));

  eid_t e3 = create_entity(&ecs);
  ASSERT_EQ (assign_component (&ecs, e3, new_array), 0);
  ASSERT_EQ (ecs.arrays[new_array].num_components, 3);
  ASSERT_EQ (ecs.arrays[new_array].map[2].entity, e3);
  ASSERT_EQ (ecs.arrays[new_array].map[2].offset, sizeof (struct some_component) * 2);

  unassign_component(&ecs, e2, new_array);
  ASSERT_EQ (ecs.arrays[new_array].num_components, 2);

  /* e3 should now be mapped to the offset e2 used to have */
  ASSERT_EQ (ecs.arrays[new_array].map[1].entity, e3);
  ASSERT_EQ (ecs.arrays[new_array].map[1].offset, sizeof (struct some_component));

  ASSERT_EQ (assign_component (&ecs, e2, new_array), 0);
  ASSERT_EQ (ecs.arrays[new_array].num_components, 3);

  /* e2 should be mapped to the last position */
  ASSERT_EQ (ecs.arrays[new_array].map[2].entity, e2);
  ASSERT_EQ (ecs.arrays[new_array].map[2].offset, sizeof (struct some_component) * 2);

  ASSERT_EQ (assign_component (&ecs, e1, new_array), 0);
  ASSERT_EQ (ecs.arrays[new_array].num_components, 4);
  ASSERT_EQ (ecs.arrays[new_array].map[3].entity, e1);
  ASSERT_EQ (ecs.arrays[new_array].map[3].offset, sizeof (struct some_component) * 3);

  unassign_component (&ecs, e1, new_array);
  ASSERT_EQ (ecs.arrays[new_array].num_components, 2);

  ASSERT_EQ (ecs.arrays[new_array].map[0].entity, e2);
  /* e2 gets mapped into the first position of e1's components */
  ASSERT_EQ (ecs.arrays[new_array].map[0].offset, 0);
}
