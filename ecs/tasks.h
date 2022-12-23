#ifndef TASKS_H_
#define TASKS_H_

#include "mat4.h"
#include "input.h"

namespace tasks {

struct KeyInputArg {
  Mat4 *mp_position_to_update;
  Input *mp_input;
};

void process_key_input(void *arg);

} // namespace tasks

#endif // TASKS_H_
