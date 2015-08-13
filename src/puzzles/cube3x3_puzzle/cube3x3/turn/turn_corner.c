#include "../cube.h"
#include "../model.h"
#include "../access.h"
#include "turn_internal.h"

static int seq[2][4] = {
	{0, 1, 2, 3},
	{3, 2, 1, 0}};

static CubeCornerId cornerQuarterRevolutions[6][4] = {
	{0, 1, 2, 3},
	{5, 6, 2, 1},
	{4, 5, 1, 0},
	{7, 4, 0, 3},
	{6, 7, 3, 2},
	{7, 6, 5, 4}};

/*
 * Gets a corner id by a face it belongs to and its index in a turn sequence.
 */
static CubeCornerId getCornerIdInTurnSequence(int i, CubeFaceId faceId, bool clockwise) {
	CubeCornerOnFaceId cornerOnFaceId;
	if (clockwise) {
		cornerOnFaceId = i;
	} else {
		cornerOnFaceId = 3 - i;
	}
	return getCornerSlotOnFace(faceId, cornerOnFaceId).id;
}

/*
 * Gets a corner by a face it belongs to and its index in a turn sequence.
 */
static struct Corner getCornerInTurnSequence(Cube cube, int i, CubeFaceId faceId, bool clockwise) {
	CubeCornerId cornerId = getCornerIdInTurnSequence(i, faceId, clockwise);
	return cube->corners[cornerId];
}

/*
 * Replaces a corner with another corner in a turn sequence.
 * 
 * Source and target corners are identified with their indices in the turn sequence.
 */
static void moveCorner(Cube cube, int source, int target, CubeFaceId faceId, bool clockwise) {
	struct Corner sourceCorner = getCornerInTurnSequence(cube, source, faceId, clockwise);
	int targetId = getCornerIdInTurnSequence(target, faceId, clockwise);
	cube->corners[targetId] = sourceCorner;
}

/*
 * Revolves the 4 corners on a face in a given direction, but does not change their rotations.
 */
void quarterRevolveCorners(Cube cube, CubeFaceId faceId, bool clockwise) {
	struct Corner tmp = getCornerInTurnSequence(cube, 3, faceId, clockwise);
	moveCorner(cube, 2, 3, faceId, clockwise);
	moveCorner(cube, 1, 2, faceId, clockwise);
	moveCorner(cube, 0, 1, faceId, clockwise);
	int finalCornerId = getCornerIdInTurnSequence(0, faceId, clockwise);
	cube->corners[finalCornerId] = tmp;
}

static CubeCornerRotation cornerQuarterRotations[6][4] = {
	{NO_ROT, NO_ROT, NO_ROT, NO_ROT},
	{CLOCKWISE_ROT, COUNTER_ROT, CLOCKWISE_ROT, COUNTER_ROT},
	{CLOCKWISE_ROT, COUNTER_ROT, CLOCKWISE_ROT, COUNTER_ROT},
	{CLOCKWISE_ROT, COUNTER_ROT, CLOCKWISE_ROT, COUNTER_ROT},
	{CLOCKWISE_ROT, COUNTER_ROT, CLOCKWISE_ROT, COUNTER_ROT},
	{NO_ROT, NO_ROT, NO_ROT, NO_ROT}};

/*
 * Rotates the 4 corners of a face to correspond with a face turn.
 * 
 * The only acceptable turn types are CLOCKWISE_TURN and COUNTER_TURN.  All others produce undefined behavior.
 */
void quarterRotateCorners(Cube cube, CubeFaceId faceId, TurnType type) {
	for (int i = 0; i < 4; i++) {
		cube->corners[cornerQuarterRevolutions[faceId][seq[(type - 1) / 2][i]]].rotation = (cube->corners[cornerQuarterRevolutions[faceId][seq[(type - 1) / 2][i]]].rotation + cornerQuarterRotations[faceId][seq[(type - 1) / 2][i]]) % 3;
	}
}