#include <libgen.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "test.h"
#include "database.h"
#include "error.h"

int main() {
  eid_t e1 = create_entity();
  eid_t e2 = create_entity();
  eid_t e3 = create_entity();
  eid_t e4 = create_entity();
  eid_t e5 = create_entity();

  assign_components(e1, XEN_GEOMETRY | XEN_MATERIAL);
  assign_components(e2, XEN_GEOMETRY | XEN_MATERIAL);
  assign_components(e3, XEN_GEOMETRY);
  assign_components(e4, XEN_GEOMETRY | XEN_MATERIAL);
  assign_components(e5, XEN_MATERIAL);

  assert(get_signature(e1) & XEN_GEOMETRY);
  assert(get_signature(e2) & XEN_GEOMETRY);
  assert(get_signature(e3) & XEN_GEOMETRY);
  assert(get_signature(e4) & XEN_GEOMETRY);

  assert(get_signature(e1) & XEN_MATERIAL);
  assert(get_signature(e2) & XEN_MATERIAL);
  assert(get_signature(e4) & XEN_MATERIAL);
  assert(get_signature(e5) & XEN_MATERIAL);
  assert(!(get_signature(e3) & XEN_MATERIAL));

  size_t num_with_geom = count_entities(XEN_GEOMETRY);
  size_t num_with_mat = count_entities(XEN_MATERIAL);
  size_t num_with_both = count_entities(XEN_GEOMETRY | XEN_MATERIAL);
  assert(num_with_geom == 4);
  assert(num_with_mat == 4);
  assert(num_with_both == 3);

  // The next 2 bock of of assertions rely on a linear foreward search. This WILL break.
  eid_t ids_with_geom[num_with_geom];
  memset(ids_with_geom, 0, sizeof(eid_t) * num_with_geom);
  get_entities(ids_with_geom, XEN_GEOMETRY);
  assert(ids_with_geom[0] == e1);
  assert(ids_with_geom[1] == e2);
  assert(ids_with_geom[2] == e3);
  assert(ids_with_geom[3] == e4);

  eid_t ids_with_mat[num_with_mat];
  memset(ids_with_mat, 0, sizeof(eid_t) * num_with_mat);
  get_entities(ids_with_mat, XEN_MATERIAL);
  assert(ids_with_mat[0] == e1);
  assert(ids_with_mat[1] == e2);
  assert(ids_with_mat[2] == e4);
  assert(ids_with_mat[3] == e5);

  remove_components(e4, XEN_MATERIAL);
  assert(!(get_signature(e4) & XEN_MATERIAL));
  num_with_geom = count_entities(XEN_GEOMETRY);
  num_with_mat = count_entities(XEN_MATERIAL);
  num_with_both = count_entities(XEN_GEOMETRY | XEN_MATERIAL);
  assert(num_with_geom == 4);
  assert(num_with_mat == 3);
  assert(num_with_both == 2);

  printf("%ld\n", sizeof(struct component));
  printf("%ld\n", sizeof(struct component) - sizeof(struct pbr_material));
  printf("%ld\n", sizeof(float) * 3);
  TEST_END();
  return 0;
}
