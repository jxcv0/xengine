#include "game_state.h"

#include "lin.h"

vec3 positions[MAX_ENTITITES];
vec3 velocities[MAX_ENTITITES];
vec3 heading[MAX_ENTITIES];      // possibly could be vec2
uint32_t mesh_id[MAX_ENTITIES];  // textures are here for now
