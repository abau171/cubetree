#include <stdbool.h>

#include "../cube.h"
#include "../model.h"
#include "../access.h"

/*
 * Checks whether a cube is solved or not.
 * 
 * Returns true on solved, false otherwise.
 */
bool cubeIsSolved(Cube cube) {
	for (int cornerSlotId = 0; cornerSlotId < NUM_CUBE_CORNERS; cornerSlotId++) {
		struct Corner corner = getCorner(cube, cornerSlotId);
		if (corner.id != cornerSlotId || corner.rotation != 0) return false;
	}
	for (int edgeSlotId = 0; edgeSlotId < NUM_CUBE_EDGES; edgeSlotId++) {
		struct Edge edge = getEdge(cube, edgeSlotId);
		if (edge.id != edgeSlotId || edge.flip != 0) return false;
	}
	return true;
}