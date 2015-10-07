#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <prune.h>

#include <cube3x3/cube.h>

struct CubeMoveNode {
	CubeFaceId faceId;
	TurnType turnType;
	struct CubeMoveNode* next;
};

struct CubeMoveNode* moveStack;

static void pushMove(CubeFaceId faceId, TurnType turnType) {
	struct CubeMoveNode* newNode = malloc(sizeof(struct CubeMoveNode));
	newNode->faceId = faceId;
	newNode->turnType = turnType;
	newNode->next = moveStack;
	moveStack = newNode;
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
				puts("push");
				pushMove(faceId, turnType);
				return true;
			}
		}
	}
	return false;
}

void solveCube(Cube cube) {
	moveStack = NULL;
	bool solved = false;
	int depth = 0;
	do {
		printf("SEARCHING %d\n", depth);
		solved = searchDepth(cube, depth);
		depth++;
	} while (!solved);
	struct CubeMoveNode* curNode = moveStack;
	while (curNode != NULL) {
		printTurn(curNode->faceId, curNode->turnType, true);
		curNode = curNode->next;
	}
}