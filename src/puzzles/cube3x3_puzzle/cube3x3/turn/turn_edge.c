#include "../cube.h"
#include "../model.h"
#include "../access.h"
#include "turn_internal.h"

/*
 * Gets an edge id by a face it belongs to and its index in a turn sequence.
 */
static struct Edge getEdgeSlotInTurnSequence(int i, CubeFaceId faceId, bool clockwise) {
	CubeEdgeOnFaceId edgeOnFaceId;
	if (clockwise) {
		edgeOnFaceId = i;
	} else {
		edgeOnFaceId = 3 - i;
	}
	return getEdgeSlotOnFace(faceId, edgeOnFaceId);
}

/*
 * Gets an edge by a face it belongs to and its index in a turn sequence.
 */
static struct Edge getEdgeInTurnSequence(Cube cube, int i, CubeFaceId faceId, bool clockwise) {
	CubeEdgeId edgeSlotId = getEdgeSlotInTurnSequence(i, faceId, clockwise).id;
	return getEdge(cube, edgeSlotId);
}

/*
 * Replaces an edge with another edge in a turn sequence.
 * 
 * Source and target edges are identified with their indices in the turn sequence.
 */
static void moveEdge(Cube cube, int source, int target, CubeFaceId faceId, bool clockwise) {
	struct Edge sourceEdge = getEdgeInTurnSequence(cube, source, faceId, clockwise);
	CubeEdgeId targetId = getEdgeSlotInTurnSequence(target, faceId, clockwise).id;
	setEdge(cube, targetId, sourceEdge);
}

/*
 * Revolves he 4 edges on a face in a given direction, but does not change their flip state.
 */
void quarterRevolveEdges(Cube cube, CubeFaceId faceId, bool clockwise) {
	struct Edge tmp = getEdgeInTurnSequence(cube, 3, faceId, clockwise);
	moveEdge(cube, 2, 3, faceId, clockwise);
	moveEdge(cube, 1, 2, faceId, clockwise);
	moveEdge(cube, 0, 1, faceId, clockwise);
	CubeEdgeId finalEdgeId = getEdgeSlotInTurnSequence(0, faceId, clockwise).id;
	setEdge(cube, finalEdgeId, tmp);
}

/*
 * Flips an edge that will be revolved along a ace from one given slot to another.
 */
static void flipEdgeAlongFace(Cube cube, int source, int target, CubeFaceId faceId, bool clockwise) {
	struct Edge sourceSlot = getEdgeSlotInTurnSequence(source, faceId, clockwise);
	struct Edge targetSlot = getEdgeSlotInTurnSequence(target, faceId, clockwise);
	CubeEdgeFlip dFlip = 2 + targetSlot.flip - sourceSlot.flip;
	flipEdge(cube, sourceSlot.id, dFlip);
}

/*
 * Flips the 4 edges of a face to correspond with a face turn.
 */
void quarterFlipEdges(Cube cube, CubeFaceId faceId, bool clockwise) {
	for (int i = 0; i < NUM_CUBE_EDGES_PER_FACE; i++) {
		int j = (i + 1) % NUM_CUBE_EDGES_PER_FACE;
		flipEdgeAlongFace(cube, i, j, faceId, clockwise);
	}
}
