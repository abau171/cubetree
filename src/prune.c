#include <stdbool.h>

#include <prune.h>
#include <cornerLookup.h>
#include <edgeLookup.h>
#include <edgeLookup2.h>

#include <cube3x3/cube.h>

bool pruneState(Cube cube, int depth) {
	return (lookupCornerDistance(cube) > depth || lookupEdgeDistance(cube) > depth || lookupEdge2Distance(cube) > depth);
}