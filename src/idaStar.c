#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <cube3x3/cube.h>
#include <cube3x3/cubeMoveStack.h>
#include <cornerLookup.h>
#include <edgeLookup.h>
#include <edgeLookup2.h>

CubeMoveStack moveStack;

static bool pruneState(Cube cube, int depth) {
	bool doPrune = false;
	doPrune = doPrune || lookupCornerDistance(cube) > depth;
	doPrune = doPrune || lookupEdgeDistance(cube) > depth;
	doPrune = doPrune || lookupEdge2Distance(cube) > depth;
	return doPrune;
}

static bool searchDepth(Cube cube, int depth, CubeFaceId lastFaceId) {
	if (depth == 0) {
		return cubeIsSolved(cube);
	}
	if (pruneState(cube, depth)) {
		return false;
	}
	for (TurnType turnType = 1; turnType < 4; turnType++) {
		for (CubeFaceId faceId = 0; faceId < 6; faceId++) {
			if (lastFaceId != faceId) {
				Cube clone = cloneCube(cube);
				turnCubeFace(clone, faceId, turnType);
				bool solved = searchDepth(clone, depth - 1, faceId);
				if (solved) {
					moveStack = pushCubeMove(moveStack, faceId, turnType);
					return true;
				}
				freeCube(clone);
			}
		}
	}
	return false;
}

void solveCube(Cube cube) {
	moveStack = newCubeMoveStack();
	bool solved = false;
	int depth = 0;
	do {
		printf("SEARCHING %d\n", depth);
		solved = searchDepth(cube, depth, NO_FACE);
		depth++;
	} while (!solved);
	while (hasCubeMove(moveStack)) {
		struct CubeMoveData move;
		moveStack = popCubeMove(moveStack, &move);
		printTurn(move, true);
	}
}