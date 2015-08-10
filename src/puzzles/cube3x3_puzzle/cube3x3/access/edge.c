#include "../cube.h"
#include "../model.h"
#include "../access.h"

/*
 * Gets the edge at a edge slot in a cube.
 */
struct Edge getEdge(Cube cube, CubeEdgeId edgeSlotId) {
	return cube->edges[edgeSlotId];
}

/*
 * Sets the edge at a edge slot in a cube.
 */
void setEdge(Cube cube, CubeEdgeId edgeSlotId, struct Edge edge) {
	cube->edges[edgeSlotId] = edge;
}

/*
 * Returns a flipped version of the given edge.
 */
struct Edge getFlippedEdge(struct Edge edge, CubeEdgeFlip dFlip) {
	edge.flip = (edge.flip + dFlip) % NUM_CUBE_FLIPS_PER_EDGE;
	return edge;
}

/*
 * Flips an edge in a cube.
 */
void flipEdge(Cube cube, CubeEdgeId edgeSlotId, CubeEdgeFlip dFlip) {
	struct Edge edge = getEdge(cube, edgeSlotId);
	struct Edge flippedEdge = getFlippedEdge(edge, dFlip);
	setEdge(cube, edgeSlotId, flippedEdge);
}