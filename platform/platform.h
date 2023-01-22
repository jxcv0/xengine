#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct window window_t;

window_t *init_window(uint32_t width, uint32_t height, const char *name);

bool should_close(window_t *window);

void set_should_close(window_t *window, bool flag);

int get_input(window_t *window);

void delete_window(window_t *window);

//
// window_t window = create_window(WIDTH, HEIGHT, WINDOW_NAME);
// if (load_opengl(window, VERSION_3_3) != 0) {
//   return 0;
// }
//
// while (!should_close(window)) {
//   uint32_t input = get_input();
//   ...
//
//   if (key_pressed(window, KEY_ESC)) {
//     ...
//     set_should_close(window);
//   }
//
//   ...
// }
//
// delete_window(window);

#endif // PLATFORM_H_
