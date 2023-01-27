#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <stdint.h>
#include "lin.h"

#define MAX_ENTITIES 1024
#define MAX_MESHES 1024

#ifdef __cplusplus
extern "C" {
#endif

enum {
  MAIN_MENU,
  GAME_PLAY,
  GAME_MENU
} game_mode = MAIN_MENU;

/**
 * @brief Reset ID free lists.
 */
void init_game_state(void);

/**
 * @brief Create create an entity and get its id.
 *
 * @return The id of the entity or 0 on failure.
 */
uint32_t create_entity(void);

/**
 * @brief Delete an entity and add its id back to the free list.
 */
void delete_entity(const uint32_t id);

/**
 * @brief Assign an entity a mesh id. TODO this mesh id is then passed to the loading system.
 *
 * @brief id The id of the entity.
 * @brief filename The name of the file storing the mesh data.
 * @return The id of the mesh or 0 on failure.
 */
uint32_t set_mesh(const uint32_t id, const char *filename);

void set_velocity(const uint32_t id, const vec3 v);

void set_position(const uint32_t id, const vec3 v);

void set_heading(const uint32_t id, const vec3 v);

/**
 * @brief Delete a mesh.
 *
 * @brief The entity id
 */
void delete_mesh(const uint32_t id);

#ifdef __cplusplus
}
#endif

#endif  // GAME_STATE_H_
