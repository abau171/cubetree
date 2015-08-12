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

/*
 * Revolves the 4 edges on a face in a given direction, but does not change their flip state.
 */
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

/*
 * Flips the 4 corners of a face to correspond with a face turn.
 * 
 * The only acceptable turn types are CLOCKWISE_TURN and COUNTER_TURN.  All others produce undefined behavior.
 */
void quarterFlipEdges(Cube cube, CubeFaceId faceId, TurnType type) {
	for (int i = 0; i < 4; i++) {
		cube->edges[edgeQuarterRevolutions[faceId][seq[(type - 1) / 2][i]]].flip = wrapEdgeFlip(cube->edges[edgeQuarterRevolutions[faceId][seq[(type - 1) / 2][i]]].flip + edgeQuarterFlips[faceId][seq[(type - 1) / 2][i]]);
	}
}