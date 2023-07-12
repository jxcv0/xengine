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

TEST (ecs_tests, set_bitset)
{
  struct component_bitset bitset;
  memset (&bitset, 0, sizeof (bitset));
  set_bitset (&bitset, 1);
  ASSERT_EQ (bitset.sets[0], (1 << 1));

  set_bitset (&bitset, 63);
  ASSERT_EQ (bitset.sets[0], (1LU << 63) + (1LU << 1));

  set_bitset (&bitset, 64);
  ASSERT_EQ (bitset.sets[1], 1);
  ASSERT_EQ (bitset.sets[0], (1LU << 63) + (1LU << 1));

  unset_bitset (&bitset, 64);
  ASSERT_EQ (bitset.sets[1], 0);
  ASSERT_EQ (bitset.sets[0], (1LU << 63) + (1LU << 1));

  unset_bitset (&bitset, 1);
  ASSERT_EQ (bitset.sets[1], 0);
  ASSERT_EQ (bitset.sets[0], (1LU << 63));
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
  ASSERT_EQ (ecs.bitsets[e1].sets[0], 1);
  ASSERT_EQ (has_component (&ecs, e1, new_array), 1);

  eid_t e2 = create_entity (&ecs);
  ASSERT_EQ (map_component (&ecs, e2, new_array), 0);
  ASSERT_EQ (ecs.arrays[new_array].num_components, 2);
  ASSERT_EQ (ecs.arrays[new_array].map[1].entity, e2);
  ASSERT_EQ (ecs.arrays[new_array].map[1].offset,
             sizeof (struct some_component));
  ASSERT_EQ (has_component (&ecs, e2, new_array), 1);

  eid_t e3 = create_entity (&ecs);
  ASSERT_EQ (map_component (&ecs, e3, new_array), 0);
  ASSERT_EQ (ecs.arrays[new_array].num_components, 3);
  ASSERT_EQ (ecs.arrays[new_array].map[2].entity, e3);
  ASSERT_EQ (ecs.arrays[new_array].map[2].offset,
             sizeof (struct some_component) * 2);
  ASSERT_EQ (has_component (&ecs, e3, new_array), 1);
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
  ASSERT_EQ (ecs.bitsets[e1].sets[0], 1);
  ASSERT_EQ (has_component (&ecs, e1, arr), 1);

  eid_t e2 = create_entity (&ecs);
  ASSERT_EQ (map_component (&ecs, e2, arr), 0);
  ASSERT_EQ (ecs.arrays[arr].num_components, 2);
  ASSERT_EQ (ecs.arrays[arr].map[1].entity, e2);
  ASSERT_EQ (ecs.arrays[arr].map[1].offset,
             sizeof (struct some_component));
  ASSERT_EQ (has_component (&ecs, e2, arr), 1);

  eid_t e3 = create_entity (&ecs);
  ASSERT_EQ (map_component (&ecs, e3, arr), 0);
  ASSERT_EQ (ecs.arrays[arr].num_components, 3);
  ASSERT_EQ (ecs.arrays[arr].map[2].entity, e3);
  ASSERT_EQ (ecs.arrays[arr].map[2].offset,
             sizeof (struct some_component) * 2);
  ASSERT_EQ (has_component (&ecs, e3, arr), 1);
  auto first_pos
      = static_cast<struct some_component *>(get_component (&ecs, e3, arr));
  first_pos->a = 42;

  unmap_component (&ecs, e2, arr);
  ASSERT_EQ (ecs.arrays[arr].num_components, 2);
  ASSERT_EQ (has_component (&ecs, e2, arr), 0);

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
  ASSERT_EQ (ecs.bitsets[e1].sets[0], 1);

  unmap_component (&ecs, e1, arr);
  ASSERT_EQ (ecs.arrays[arr].num_components, 2);
  ASSERT_EQ (get_component (&ecs, e1, arr), nullptr);
  ASSERT_EQ (ecs.bitsets[e1].sets[0], 0);
  ASSERT_EQ (has_component (&ecs, e1, arr), 0);

  ASSERT_EQ (ecs.arrays[arr].map[0].entity, e2);
  /* e2 gets mapped into the first position of e1's components */
  ASSERT_EQ (ecs.arrays[arr].map[0].offset, 0);
}

TEST (ecs_tests, count_archetype)
{
  struct ecs ecs;
  memset (&ecs, 0, sizeof (ecs));
  cid_t c1 = create_component_array (&ecs, sizeof (struct some_component), 10);
  cid_t c2 = create_component_array (&ecs, sizeof (struct some_component), 5);
  cid_t c3 = create_component_array (&ecs, sizeof (struct some_component), 5);

  eid_t e1 = create_entity (&ecs);
  eid_t e2 = create_entity (&ecs);
  eid_t e3 = create_entity (&ecs);

  cid_t cids[3] = {c1, c2, c3}; 
  ASSERT_EQ (is_archetype (&ecs, e1, 3, cids), 0);
  ASSERT_EQ (is_archetype (&ecs, e2, 3, cids), 0);
  ASSERT_EQ (is_archetype (&ecs, e3, 3, cids), 0);
  ASSERT_EQ (count_archetype (&ecs, 3, cids), 0);

  map_component (&ecs, e1, c1);
  ASSERT_EQ (is_archetype (&ecs, e1, 1, &c1), 1);
  ASSERT_EQ (is_archetype (&ecs, e1, 3, cids), 0);
  ASSERT_EQ (is_archetype (&ecs, e2, 3, cids), 0);
  ASSERT_EQ (is_archetype (&ecs, e3, 3, cids), 0);
  ASSERT_EQ (count_archetype (&ecs, 3, cids), 0);
  ASSERT_EQ (count_archetype (&ecs, 1, cids), 1);

  map_component (&ecs, e1, c2);
  map_component (&ecs, e1, c3);
  ASSERT_EQ (is_archetype (&ecs, e1, 3, cids), 1);
  ASSERT_EQ (is_archetype (&ecs, e2, 3, cids), 0);
  ASSERT_EQ (is_archetype (&ecs, e3, 3, cids), 0);
  ASSERT_EQ (count_archetype (&ecs, 3, cids), 1);

  map_component (&ecs, e2, c1);
  map_component (&ecs, e2, c2);
  ASSERT_EQ (is_archetype (&ecs, e1, 3, cids), 1);
  ASSERT_EQ (is_archetype (&ecs, e2, 2, cids), 1);
  ASSERT_EQ (is_archetype (&ecs, e2, 3, cids), 0);
  ASSERT_EQ (is_archetype (&ecs, e3, 3, cids), 0);

  map_component (&ecs, e3, c1);
  map_component (&ecs, e3, c2);
  map_component (&ecs, e3, c3);
  ASSERT_EQ (is_archetype (&ecs, e1, 3, cids), 1);
  ASSERT_EQ (is_archetype (&ecs, e2, 3, cids), 0);
  ASSERT_EQ (is_archetype (&ecs, e3, 3, cids), 1);
  unmap_component (&ecs, e3, c3);
  ASSERT_EQ (is_archetype (&ecs, e1, 3, cids), 1);
  ASSERT_EQ (is_archetype (&ecs, e2, 3, cids), 0);
  ASSERT_EQ (is_archetype (&ecs, e3, 3, cids), 0);
}

