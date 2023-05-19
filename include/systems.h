#ifndef SYSTEMS_
#define SYSTEMS_

#include "mem.h"
#include "pbr_deferred.h"

// void sys_update_model_matrices(void);

void sys_render_geometries(struct renderer *r, float projection[4][4],
                           float view[4][4]);

#endif  // SYSTEMS_
