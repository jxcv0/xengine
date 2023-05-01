#include <assert.h>

#include "buffer.h"

#define BUF_SIZE 16

struct some_struct {
    size_t s;
};

struct buffer b;

int main() {

    assert(gen_buffer(&b, BUF_SIZE, sizeof(struct some_struct)) == 0);
    assert(b.size == BUF_SIZE);
    assert(b.count == 0);
    assert(b.data != NULL);
    assert(b.stride == sizeof(struct some_struct));

    struct some_struct s;

    s.s = 0;
    handle_t h1 = new_handle();
    assert(buffer_insert(&b, &s, h1) != -1);

    s.s = 1;
    handle_t h2 = new_handle();
    assert(buffer_insert(&b, &s, h2) != -1);

    s.s = 2;
    handle_t h3 = new_handle();
    assert(buffer_insert(&b, &s, h3) != -1);
}
