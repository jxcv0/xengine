#include "database.h"

#include "error.h"

/**
 * @brief Strores the signatures of each entity.
 */
static size_t num_entities = 0;
struct {
  eid_t m_entity;
  sig_t m_signature;
} entity_table[MAX_NUM_ENTITIES] = {0};

struct component {
  eid_t m_entity;
  union {
    struct geometry m_geom;
    struct pbr_material m_mat;
    /* others */
  };
};

static struct component geometries[MAX_NUM_GEOMETRIES] = {0};
static size_t num_geometries = 0;

static struct component materials[MAX_NUM_MATERIALS] = {0};
static size_t num_materials = 0;

/**
 * ----------------------------------------------------------------------------
 */
eid_t create_entity(void) {
  static eid_t id_counter = 1;
  ASSERT(id_counter != MAX_ENTITY_ID, "Maximum entity ID value reached.\n");
  ASSERT(num_entities != MAX_NUM_ENTITIES, "Maximum value of eid reached.\n");
  eid_t id = id_counter++;

  entity_table[num_entities].m_entity = id;
  entity_table[num_entities].m_signature = 0;
  ++num_entities;

  return id;
}

/**
 * ----------------------------------------------------------------------------
 */
void delete_entity(const eid_t id) {
  (void)id;
  // TODO big ol switch?
}

/**
 * ----------------------------------------------------------------------------
 */
sig_t get_signature(const eid_t id) {
  // TODO omp
  sig_t sig = 0;
  const size_t n = num_entities;
  for (size_t i = 0; i < n; i++) {
    if (entity_table[i].m_entity == id) {
      sig = entity_table[i].m_signature;
      break;
    }
  }
  return sig;
}

/**
 * ----------------------------------------------------------------------------
 */
static void set_signature(const eid_t id, const sig_t sig) {
  const size_t n = num_entities;
  for (size_t i = 0; i < n; i++) {
    if (entity_table[i].m_entity == id) {
      entity_table[i].m_signature |= sig;
      return;
    }
  }
}

/**
 * ----------------------------------------------------------------------------
 */
size_t count_entities(const sig_t sig) {
  // TODO omp
  size_t count = 0;
  const size_t n = num_entities;
  for (size_t i = 0; i < n; i++) {
    if ((entity_table[i].m_signature & sig) != 0) {
      ++count;
    }
  }
  return count;
}

/**
 * ----------------------------------------------------------------------------
 */
static void create_component(struct component* arr, size_t* count, const size_t max,
                    const eid_t id) {
  ASSERT(*count != max, "Maximum number of component type reached.\n");
  arr[(*count)++].m_entity = id;
}

/**
 * ----------------------------------------------------------------------------
 */
static void delete_component(struct component* arr, size_t* count, const eid_t id) {
  const size_t n = *count;
  for (size_t i = 0; i < n; i++) {
    if (arr[i].m_entity == id) {
      // swap with last then decrement index
      arr[i] = arr[n];
      --(*count);
      return;
    }
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void assign_component(const eid_t id, const sig_t sig) {
  switch (sig) {
    case GEOMETRY:
      create_component(geometries, &num_geometries, MAX_NUM_GEOMETRIES, id);
      set_signature(id, sig);
      break;

    case MATERIAL:
      create_component(materials, &num_materials, MAX_NUM_MATERIALS, id);
      set_signature(id, sig);
      break;

    default:
      break;
  }
}
