#ifndef CUBE_INTERNAL_H
#define CUBE_INTERNAL_H

#include <stdint.h>

struct Corner {
	int_fast8_t id;
	int_fast8_t rotation;
};

struct Edge {
	int_fast8_t id;
	int_fast8_t flip;
};

struct Cube {
	struct Corner corners[8];
	struct Edge edges[12];
};

#endif
