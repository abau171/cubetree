#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <cube3x3/cube.h>
#include <cube3x3/cubeMoveStack.h>

CubeMoveStack moveStack;

static bool pruneState(Cube cube, int depth) {
	(void) cube;
	(void) depth;
	return false;
}

static bool searchDepth(Cube cube, int depth) {
	if (depth == 0) {
		return cubeIsSolved(cube);
	}
	if (pruneState(cube, depth)) {
		return false;
	}
	for (TurnType turnType = 1; turnType < 4; turnType++) {
		for (CubeFaceId faceId = 0; faceId < 6; faceId++) {
			Cube clone = cloneCube(cube);
			turnCubeFace(clone, faceId, turnType);
			bool solved = searchDepth(clone, depth - 1);
			if (solved) {
				moveStack = pushCubeMove(moveStack, faceId, turnType);
				return true;
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
		solved = searchDepth(cube, depth);
		depth++;
	} while (!solved);
	while (hasCubeMove(moveStack)) {
		struct CubeMoveData move;
		moveStack = popCubeMove(moveStack, &move);
		printTurn(move, true);
	}
}