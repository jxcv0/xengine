#ifndef SYSTEMS_H_
#define SYSTEMS_H_

#include "gamestate.h"
#include "pbr_deferred.h"

void sys_update_model_matrices(void);

void sys_load(uint64_t attrib_type);

void sys_load_materials(void);

void sys_render_geometries(struct renderer *r, mat4_t projection, mat4_t view);

#endif  // SYSTEMS_H_
