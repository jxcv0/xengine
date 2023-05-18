#ifndef SYSTEMS_
#define SYSTEMS_

#include "assets.h"
#include "mem.h"
#include "pbr_deferred.h"

void sys_process_queued_asset_loads(void);

void sys_render(void);

#endif  // SYSTEMS_
