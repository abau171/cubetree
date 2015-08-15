#include "../cube.h"
#include "../model.h"
#include "../access.h"
#include "turn_internal.h"

/*
 * Gets a corner id by a face it belongs to and its index in a turn sequence.
 */
static struct Corner getCornerSlotInTurnSequence(int i, CubeFaceId faceId, bool clockwise) {
	CubeCornerOnFaceId cornerOnFaceId;
	if (clockwise) {
		cornerOnFaceId = i;
	} else {
		cornerOnFaceId = 3 - i;
	}
	return getCornerSlotOnFace(faceId, cornerOnFaceId);
}

/*
 * Gets a corner by a face it belongs to and its index in a turn sequence.
 */
static struct Corner getCornerInTurnSequence(Cube cube, int i, CubeFaceId faceId, bool clockwise) {
	CubeCornerId cornerSlotId = getCornerSlotInTurnSequence(i, faceId, clockwise).id;
	return getCorner(cube, cornerSlotId);
}

/*
 * Replaces a corner with another corner in a turn sequence.
 * 
 * Source and target corners are identified with their indices in the turn sequence.
 */
static void moveCorner(Cube cube, int source, int target, CubeFaceId faceId, bool clockwise) {
	struct Corner sourceCorner = getCornerInTurnSequence(cube, source, faceId, clockwise);
	CubeCornerId targetId = getCornerSlotInTurnSequence(target, faceId, clockwise).id;
	setCorner(cube, targetId, sourceCorner);
}

/*
 * Revolves the 4 corners on a face in a given direction, but does not change their rotations.
 */
void quarterRevolveCorners(Cube cube, CubeFaceId faceId, bool clockwise) {
	struct Corner tmp = getCornerInTurnSequence(cube, 3, faceId, clockwise);
	moveCorner(cube, 2, 3, faceId, clockwise);
	moveCorner(cube, 1, 2, faceId, clockwise);
	moveCorner(cube, 0, 1, faceId, clockwise);
	CubeCornerId finalCornerId = getCornerSlotInTurnSequence(0, faceId, clockwise).id;
	setCorner(cube, finalCornerId, tmp);
}

static void rotateCornerAlongFace(Cube cube, int source, int target, CubeFaceId faceId, bool clockwise) {
	struct Corner sourceCorner = getCornerInTurnSequence(cube, source, faceId, clockwise);
	struct Corner sourceSlot = getCornerSlotInTurnSequence(source, faceId, clockwise);
	struct Corner targetSlot = getCornerSlotInTurnSequence(target, faceId, clockwise);
	CubeCornerRotation dRotation = 3 - targetSlot.rotation + sourceSlot.rotation;
	rotateCorner(cube, sourceSlot.id, dRotation);
}

/*
 * Rotates the 4 corners of a face to correspond with a face turn.
 */
void quarterRotateCorners(Cube cube, CubeFaceId faceId, bool clockwise) {
	for (int i = 0; i < NUM_CUBE_CORNERS_PER_FACE; i++) {
		int j = (i + 1) % NUM_CUBE_CORNERS_PER_FACE;
		rotateCornerAlongFace(cube, i, j, faceId, clockwise);
	}
}