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
static size_t get_id_index(const eid_t id) {
  const size_t n = num_entities;
  size_t i;
  for (i = 0; i < n; i++) {
    if (entity_table[i].m_entity == id) {
      break;
    }
  }
  return i;
}

/**
 * ----------------------------------------------------------------------------
 */
static void set_sig_bits(const eid_t id, const sig_t sig) {
  const size_t i = get_id_index(id);
  if (i != num_entities) {
    entity_table[i].m_signature |= sig;
  }
}

/**
 * ----------------------------------------------------------------------------
 */
static void unset_sig_bits(const eid_t id, const sig_t sig) {
  const size_t i = get_id_index(id);
  if (i != num_entities) {
    entity_table[i].m_signature ^= sig;
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
    if ((entity_table[i].m_signature & sig) == sig) {
      ++count;
    }
  }
  return count;
}

/**
 * ----------------------------------------------------------------------------
 */
void get_entities(eid_t* arr, const sig_t sig) {
  size_t count = 0;
  const size_t n = num_entities;
  for (size_t i = 0; i < n; i++) {
    if ((entity_table[i].m_signature & sig) == sig) {
      arr[count++] = entity_table[i].m_entity;
    }
  }
}

/**
 * ----------------------------------------------------------------------------
 */
static void create_component(struct component* arr, size_t* count,
                             const size_t max, const eid_t id) {
  ASSERT(*count != max, "Maximum number of component type reached.\n");
  arr[(*count)++].m_entity = id;
}

/**
 * ----------------------------------------------------------------------------
 */
void delete_component(struct component* arr, size_t* count, const eid_t id) {
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
void assign_components(const eid_t id, const sig_t sig) {
  if ((sig & XEN_GEOMETRY) != 0) {
    create_component(geometries, &num_geometries, MAX_NUM_GEOMETRIES, id);
    set_sig_bits(id, XEN_GEOMETRY);
  }

  if ((sig & XEN_MATERIAL) != 0) {
    create_component(materials, &num_materials, MAX_NUM_MATERIALS, id);
    set_sig_bits(id, XEN_MATERIAL);
  }

  /* TODO other component types*/
}

/**
 * ----------------------------------------------------------------------------
 */
void remove_components(const eid_t id, const sig_t sig) {
  if ((sig & XEN_GEOMETRY) != 0) {
    delete_component(geometries, &num_geometries, id);
    unset_sig_bits(id, XEN_GEOMETRY);
  }

  if ((sig & XEN_MATERIAL) != 0) {
    delete_component(materials, &num_materials, id);
    unset_sig_bits(id, XEN_MATERIAL);
  }

  /* TODO other component types*/
}
