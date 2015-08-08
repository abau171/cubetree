#include "../cube.h"
#include "../model.h"
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

static CubeCornerId getCornerIdFromFaceIndex(CubeFaceId faceId, int faceIndex) {
	return cornerQuarterRevolutions[faceId][faceIndex];
}

static int getCornerIdInTurnSequence(int i, CubeFaceId faceId, bool clockwise) {
	int faceIndex;
	if (clockwise) {
		faceIndex = i;
	} else {
		faceIndex = 3 - i;
	}
	return getCornerIdFromFaceIndex(faceId, faceIndex);
}

static struct Corner getCornerInTurnSequence(Cube cube, int i, CubeFaceId faceId, bool clockwise) {
	CubeCornerId cornerId = getCornerIdInTurnSequence(i, faceId, clockwise);
	return cube->corners[cornerId];
}

static void moveCorner(Cube cube, int source, int target, CubeFaceId faceId, bool clockwise) {
	struct Corner sourceCorner = getCornerInTurnSequence(cube, source, faceId, clockwise);
	int targetId = getCornerIdInTurnSequence(target, faceId, clockwise);
	cube->corners[targetId] = sourceCorner;
}

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

void quarterRotateCorners(Cube cube, CubeFaceId faceId, TurnType type) {
	for (int i = 0; i < 4; i++) {
		cube->corners[cornerQuarterRevolutions[faceId][seq[(type - 1) / 2][i]]].rotation = wrapCornerRotation(cube->corners[cornerQuarterRevolutions[faceId][seq[(type - 1) / 2][i]]].rotation + cornerQuarterRotations[faceId][seq[(type - 1) / 2][i]]);
	}
}