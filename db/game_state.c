#include "game_state.h"
#include "id_list.h"

// TODO input
uint32_t entity_count = 0;
uint32_t mesh_count = 0;
uint32_t entity_id_list[MAX_ENTITIES] = {0};
uint32_t mesh_id_list[MAX_MESHES] = {0};

vec3 positions[MAX_ENTITITES] = {0};
vec3 velocities[MAX_ENTITITES] = {0};
vec3 heading[MAX_ENTITIES] = {0};      // possibly could be vec2
uint32_t mesh_id[MAX_MESHES] = {0};  // textures are here for now

/**
 * ----------------------------------------------------------------------------
 */
void init_game_state(void) {
  init_id_list(entity_id_list, &entity_count, MAX_ENTITIES);
  init_id_list(mesh_id_list, &mesh_count, MAX_MESHES);
}

/**
 * ----------------------------------------------------------------------------
 */
uint32_t create_entity(void) {
  pop_id(entity_id_list, &entity_count, MAX_ENTITIES);
}

/**
 * ----------------------------------------------------------------------------
 */
void delete_entity(const uint32_t id) {
  push_id(entity_id_list, &entity_count, MAX_ENTITIES);
}

/**
 * ----------------------------------------------------------------------------
 */
uint32_t set_mesh(const uint32_t id, const char *filename) {
  // TODO notify loading system
  return pop_id(mesh_id_list, &mesh_count, MAX_MESHES);
}

/**
 * ----------------------------------------------------------------------------
 */
void delete_mesh(const uint32_t id) {
  // TODO notify loading system
  push_id(mesh_id_list, &mesh_count, MAX_MESHES);
}

/**
 * uint32_t player_id = create_entity();
 * uint32_t player_mesh_id = set_mesh(player_id, "player_mesh.model");
 * set_velocity(player_id, PLAYER_START_VELOCITY);
 * set_position(player_id, PLAYER_START_POSITION);
 * set_heading(player_id, PLAYER_START_HEADING);
 *
 * ... other settup ...
 *
 * run_all_systems();
 */
