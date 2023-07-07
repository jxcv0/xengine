#ifndef VERTEX_ALLOC_H_
#define VERTEX_ALLOC_H_

#include "asset_types.h"

#ifdef __cplusplus
extern "C" {
#endif

enum block_state
{
  block_state_FREE,
  block_state_ALLOCATED,
  block_state_SPLIT
};

struct vertex_block
{
  struct vertex *start;
  struct vertex_block *buddy;
  unsigned int size;
  int state;
};


struct vertex_allocator
{
  struct vertex_block root_block;
};

struct vertex *init_vertex_buffer (struct vertex_allocator *alloc);

struct vertex *alloc_vertices (struct vertex_allocator *alloc, unsigned int count);

#ifdef __cplusplus
}
#endif


#endif  // VERTEX_ALLOC_H_
