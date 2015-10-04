#include <stdbool.h>

#include "cube3x3/cube.h"
#include "prune.h"
#include "cornerLookup.h"

bool pruneState(Cube cube, int depth) {
	return (lookupCornerDistance(cube) > depth);
}