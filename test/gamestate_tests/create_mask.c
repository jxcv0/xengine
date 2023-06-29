#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>

#include "gamestate.h"
#include "test.h"

int main(int argc, char **argv) {
	test_start(argc, argv);
	uint64_t mesh_bit = (1LL << attrib_type_MESH);
	uint64_t mat_bit  = (1LL << attrib_type_MATERIAL);
	uint64_t pos_bit  = (1LL << attrib_type_POSITION);

	uint64_t a[] = {attrib_type_MESH, attrib_type_MATERIAL, attrib_type_POSITION};
	uint64_t m   = create_mask(3, a);

	ASSERT_INT_EQ(m, (mesh_bit | mat_bit | pos_bit));
	test_end();
	return 0;
}