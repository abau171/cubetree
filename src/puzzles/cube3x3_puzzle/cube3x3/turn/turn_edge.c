#include "../cube.h"
#include "../model.h"
#include "../access.h"
#include "turn_internal.h"

static struct Edge getEdgeSlotInTurnSequence(int i, CubeFaceId faceId, bool clockwise) {
	CubeEdgeOnFaceId edgeOnFaceId;
	if (clockwise) {
		edgeOnFaceId = i;
	} else {
		edgeOnFaceId = 3 - i;
	}
	return getEdgeSlotOnFace(faceId, edgeOnFaceId);
}

static struct Edge getEdgeInTurnSequence(Cube cube, int i, CubeFaceId faceId, bool clockwise) {
	CubeEdgeId edgeSlotId = getEdgeSlotInTurnSequence(i, faceId, clockwise).id;
	return getEdge(cube, edgeSlotId);
}

static void moveEdge(Cube cube, int source, int target, CubeFaceId faceId, bool clockwise) {
	struct Edge sourceEdge = getEdgeInTurnSequence(cube, source, faceId, clockwise);
	CubeEdgeId targetId = getEdgeSlotInTurnSequence(target, faceId, clockwise).id;
	setEdge(cube, targetId, sourceEdge);
}

void quarterRevolveEdges(Cube cube, CubeFaceId faceId, bool clockwise) {
	struct Edge tmp = getEdgeInTurnSequence(cube, 3, faceId, clockwise);
	moveEdge(cube, 2, 3, faceId, clockwise);
	moveEdge(cube, 1, 2, faceId, clockwise);
	moveEdge(cube, 0, 1, faceId, clockwise);
	CubeEdgeId finalEdgeId = getEdgeSlotInTurnSequence(0, faceId, clockwise).id;
	setEdge(cube, finalEdgeId, tmp);
}

static void flipEdgeAlongFace(Cube cube, int source, int target, CubeFaceId faceId, bool clockwise) {
	struct Edge sourceEdge = getEdgeInTurnSequence(cube, source, faceId, clockwise);
	struct Edge sourceSlot = getEdgeSlotInTurnSequence(source, faceId, clockwise);
	struct Edge targetSlot = getEdgeSlotInTurnSequence(target, faceId, clockwise);
	CubeEdgeFlip dFlip = 2 + targetSlot.flip - sourceSlot.flip;
	flipEdge(cube, sourceSlot.id, dFlip);
}

void quarterFlipEdges(Cube cube, CubeFaceId faceId, bool clockwise) {
	for (int i = 0; i < NUM_CUBE_EDGES_PER_FACE; i++) {
		int j = (i + 1) % NUM_CUBE_EDGES_PER_FACE;
		flipEdgeAlongFace(cube, i, j, faceId, clockwise);
	}
}
