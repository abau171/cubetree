#include "cube.h"
#include "cube_internal.h"

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

static void quarterRevolveCorners(Cube cube, CubeFaceId faceId, TurnType type) {
	struct Corner tmp = cube->corners[cornerQuarterRevolutions[faceId][seq[(type - 1) / 2][3]]];
	cube->corners[cornerQuarterRevolutions[faceId][seq[(type - 1) / 2][3]]] = cube->corners[cornerQuarterRevolutions[faceId][seq[(type - 1) / 2][2]]];
	cube->corners[cornerQuarterRevolutions[faceId][seq[(type - 1) / 2][2]]] = cube->corners[cornerQuarterRevolutions[faceId][seq[(type - 1) / 2][1]]];
	cube->corners[cornerQuarterRevolutions[faceId][seq[(type - 1) / 2][1]]] = cube->corners[cornerQuarterRevolutions[faceId][seq[(type - 1) / 2][0]]];
	cube->corners[cornerQuarterRevolutions[faceId][seq[(type - 1) / 2][0]]] = tmp;
}

static CubeCornerRotation cornerQuarterRotations[6][4] = {
	{NO_ROT, NO_ROT, NO_ROT, NO_ROT},
	{CLOCKWISE_ROT, COUNTER_ROT, CLOCKWISE_ROT, COUNTER_ROT},
	{CLOCKWISE_ROT, COUNTER_ROT, CLOCKWISE_ROT, COUNTER_ROT},
	{CLOCKWISE_ROT, COUNTER_ROT, CLOCKWISE_ROT, COUNTER_ROT},
	{CLOCKWISE_ROT, COUNTER_ROT, CLOCKWISE_ROT, COUNTER_ROT},
	{NO_ROT, NO_ROT, NO_ROT, NO_ROT}};

static void quarterRotateCorners(Cube cube, CubeFaceId faceId, TurnType type) {
	for (int i = 0; i < 4; i++) {
		cube->corners[cornerQuarterRevolutions[faceId][seq[(type - 1) / 2][i]]].rotation = wrapCornerRotation(cube->corners[cornerQuarterRevolutions[faceId][seq[(type - 1) / 2][i]]].rotation + cornerQuarterRotations[faceId][seq[(type - 1) / 2][i]]);
	}
}

static CubeEdgeId edgeQuarterRevolutions[6][4] = {
	{0, 1, 2, 3},
	{9, 6, 1, 5},
	{8, 5, 0, 4},
	{11, 4, 3, 7},
	{10, 7, 2, 6},
	{10, 9, 8, 11}};

static void quarterRevolveEdges(Cube cube, CubeFaceId faceId, TurnType type) {
	struct Edge tmp = cube->edges[edgeQuarterRevolutions[faceId][seq[(type - 1) / 2][3]]];
	cube->edges[edgeQuarterRevolutions[faceId][seq[(type - 1) / 2][3]]] = cube->edges[edgeQuarterRevolutions[faceId][seq[(type - 1) / 2][2]]];
	cube->edges[edgeQuarterRevolutions[faceId][seq[(type - 1) / 2][2]]] = cube->edges[edgeQuarterRevolutions[faceId][seq[(type - 1) / 2][1]]];
	cube->edges[edgeQuarterRevolutions[faceId][seq[(type - 1) / 2][1]]] = cube->edges[edgeQuarterRevolutions[faceId][seq[(type - 1) / 2][0]]];
	cube->edges[edgeQuarterRevolutions[faceId][seq[(type - 1) / 2][0]]] = tmp;
}

static CubeEdgeFlip edgeQuarterFlips[6][4] = {
	{NO_FLIP, NO_FLIP, NO_FLIP, NO_FLIP},
	{NO_FLIP, NO_FLIP, NO_FLIP, NO_FLIP},
	{FLIP, FLIP, FLIP, FLIP},
	{NO_FLIP, NO_FLIP, NO_FLIP, NO_FLIP},
	{FLIP, FLIP, FLIP, FLIP},
	{NO_FLIP, NO_FLIP, NO_FLIP, NO_FLIP}};

static void quarterFlipEdges(Cube cube, CubeFaceId faceId, TurnType type) {
	for (int i = 0; i < 4; i++) {
		cube->edges[edgeQuarterRevolutions[faceId][seq[(type - 1) / 2][i]]].flip = wrapEdgeFlip(cube->edges[edgeQuarterRevolutions[faceId][seq[(type - 1) / 2][i]]].flip + edgeQuarterFlips[faceId][seq[(type - 1) / 2][i]]);
	}
}

void turnCubeFace(Cube cube, CubeFaceId faceId, TurnType type) {
	switch (type) {
	case DOUBLE_TURN:
		turnCubeFace(cube, faceId, CLOCKWISE_TURN);
		turnCubeFace(cube, faceId, CLOCKWISE_TURN);
		break;
	case CLOCKWISE_TURN:
	case COUNTER_TURN:
		quarterRotateCorners(cube, faceId, type);
		quarterRevolveCorners(cube, faceId, type);
		quarterFlipEdges(cube, faceId, type);
		quarterRevolveEdges(cube, faceId, type);
		break;
	default:
		break;
	}
}
