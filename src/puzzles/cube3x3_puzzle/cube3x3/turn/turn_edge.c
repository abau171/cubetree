#include "../cube.h"
#include "../model.h"
#include "turn_internal.h"

static int seq[2][4] = {
	{0, 1, 2, 3},
	{3, 2, 1, 0}};

static CubeEdgeId edgeQuarterRevolutions[6][4] = {
	{0, 1, 2, 3},
	{9, 6, 1, 5},
	{8, 5, 0, 4},
	{11, 4, 3, 7},
	{10, 7, 2, 6},
	{10, 9, 8, 11}};

void quarterRevolveEdges(Cube cube, CubeFaceId faceId, TurnType type) {
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

void quarterFlipEdges(Cube cube, CubeFaceId faceId, TurnType type) {
	for (int i = 0; i < 4; i++) {
		cube->edges[edgeQuarterRevolutions[faceId][seq[(type - 1) / 2][i]]].flip = wrapEdgeFlip(cube->edges[edgeQuarterRevolutions[faceId][seq[(type - 1) / 2][i]]].flip + edgeQuarterFlips[faceId][seq[(type - 1) / 2][i]]);
	}
}