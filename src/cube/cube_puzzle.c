#include <stdbool.h>

#include "../puzzle.h"
#include "model/cube.h"

void* generateStartState() {
	return newCube();
}

int getNumPossibleMoves() {
	return 18;
}

static bool cubeStatePruner(void* state) {
	return false;
}

StatePruningFunction getStatePruningFunction() {
	return &cubeStatePruner;
}

static bool cubeMovePruner(void* state, int moveId) {
	return false;
}

MovePruningFunction getMovePruningFunction() {
	return &cubeMovePruner;
}

void makeMove(void* state, int moveId) {
	CubeFaceId faceId = moveId % 6;
	TurnType type = (moveId - faceId) / 6 + 1;
	turnCubeFace(state, faceId, type);
}

void undoMove(void* state, int moveId) {
	int newMoveId;
	if (moveId < 6) newMoveId = moveId + 12;
	else if (moveId >= 12) newMoveId = moveId - 12;
	else moveId = moveId;
	makeMove(state, newMoveId);
}

bool isSolved(void* state) {
	return cubeIsSolved(state);
}
