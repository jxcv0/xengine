#include "ecs.h"
#include <gtest/gtest.h>
#include <string.h>

struct some_component
{
  int a;
  float b;
};

TEST (ecs_tests, create_component_array)
{
  struct ecs ecs;
  memset (&ecs, 0, sizeof (ecs));
  cid_t new_array
      = create_component_array (&ecs, sizeof (struct some_component), 20);
  ASSERT_EQ (new_array, 0);
  ASSERT_EQ (ecs.num_component_types, 1);
  ASSERT_NE (ecs.arrays[0].buf, nullptr);
  ASSERT_EQ (ecs.arrays[0].bufsize, sizeof (struct some_component) * 20);
  ASSERT_EQ (ecs.arrays[0].num_components, 0);
  ASSERT_EQ (ecs.arrays[0].stride, sizeof (struct some_component));
}

TEST (ecs_tests, map_component)
{
  struct ecs ecs;
  memset (&ecs, 0, sizeof (ecs));
  eid_t e1 = create_entity (&ecs);
  cid_t new_array
      = create_component_array (&ecs, sizeof (struct some_component), 20);

  ASSERT_EQ (map_component (&ecs, e1, new_array), 0);
  ASSERT_EQ (ecs.arrays[new_array].num_components, 1);
  ASSERT_EQ (ecs.arrays[new_array].map[0].entity, e1);
  ASSERT_EQ (ecs.arrays[new_array].map[0].offset, 0);

  eid_t e2 = create_entity (&ecs);
  ASSERT_EQ (map_component (&ecs, e2, new_array), 0);
  ASSERT_EQ (ecs.arrays[new_array].num_components, 2);
  ASSERT_EQ (ecs.arrays[new_array].map[1].entity, e2);
  ASSERT_EQ (ecs.arrays[new_array].map[1].offset,
             sizeof (struct some_component));

  eid_t e3 = create_entity (&ecs);
  ASSERT_EQ (map_component (&ecs, e3, new_array), 0);
  ASSERT_EQ (ecs.arrays[new_array].num_components, 3);
  ASSERT_EQ (ecs.arrays[new_array].map[2].entity, e3);
  ASSERT_EQ (ecs.arrays[new_array].map[2].offset,
             sizeof (struct some_component) * 2);
}

TEST (ecs_tests, unmap_component)
{
  struct ecs ecs;
  memset (&ecs, 0, sizeof (ecs));
  cid_t arr
      = create_component_array (&ecs, sizeof (struct some_component), 20);

  eid_t e1 = create_entity (&ecs);
  ASSERT_EQ (map_component (&ecs, e1, arr), 0);
  ASSERT_EQ (ecs.arrays[arr].num_components, 1);
  ASSERT_EQ (ecs.arrays[arr].map[0].entity, e1);
  ASSERT_EQ (ecs.arrays[arr].map[0].offset, 0);

  eid_t e2 = create_entity (&ecs);
  ASSERT_EQ (map_component (&ecs, e2, arr), 0);
  ASSERT_EQ (ecs.arrays[arr].num_components, 2);
  ASSERT_EQ (ecs.arrays[arr].map[1].entity, e2);
  ASSERT_EQ (ecs.arrays[arr].map[1].offset,
             sizeof (struct some_component));

  eid_t e3 = create_entity (&ecs);
  ASSERT_EQ (map_component (&ecs, e3, arr), 0);
  ASSERT_EQ (ecs.arrays[arr].num_components, 3);
  ASSERT_EQ (ecs.arrays[arr].map[2].entity, e3);
  ASSERT_EQ (ecs.arrays[arr].map[2].offset,
             sizeof (struct some_component) * 2);
  auto first_pos
      = static_cast<struct some_component *>(get_component (&ecs, e3, arr));
  first_pos->a = 42;

  unmap_component (&ecs, e2, arr);
  ASSERT_EQ (ecs.arrays[arr].num_components, 2);

  /* e3 should now be mapped to the offset e2 used to have */
  ASSERT_EQ (ecs.arrays[arr].map[1].entity, e3);
  ASSERT_EQ (ecs.arrays[arr].map[1].offset,
             sizeof (struct some_component));
  auto second_pos = static_cast<struct some_component *>(get_component (&ecs, e3, arr));
  ASSERT_NE (first_pos, second_pos);
  ASSERT_EQ (second_pos->a, 42);

  ASSERT_EQ (map_component (&ecs, e2, arr), 0);
  ASSERT_EQ (ecs.arrays[arr].num_components, 3);

  /* e2 should be mapped to the last position */
  ASSERT_EQ (ecs.arrays[arr].map[2].entity, e2);
  ASSERT_EQ (ecs.arrays[arr].map[2].offset,
             sizeof (struct some_component) * 2);

  ASSERT_EQ (map_component (&ecs, e1, arr), 0);
  ASSERT_EQ (ecs.arrays[arr].num_components, 4);
  ASSERT_EQ (ecs.arrays[arr].map[3].entity, e1);
  ASSERT_EQ (ecs.arrays[arr].map[3].offset,
             sizeof (struct some_component) * 3);

  unmap_component (&ecs, e1, arr);
  ASSERT_EQ (ecs.arrays[arr].num_components, 2);
  ASSERT_EQ (get_component (&ecs, e1, arr), nullptr);

  ASSERT_EQ (ecs.arrays[arr].map[0].entity, e2);
  /* e2 gets mapped into the first position of e1's components */
  ASSERT_EQ (ecs.arrays[arr].map[0].offset, 0);
}
