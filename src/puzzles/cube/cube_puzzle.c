#include <stdbool.h>

#include "../../puzzle_module.h"
#include "model/cube.h"

static int numPossibleMoves = 18;

static void* getStartState() {
	return newCube();
}

static bool pruneState(void* state) {
	return false;
}

static bool pruneMove(void* state, int moveId) {
	return false;
}

static void makeMove(void* state, int moveId) {
	CubeFaceId faceId = moveId % 6;
	TurnType type = (moveId - faceId) / 6 + 1;
	turnCubeFace(state, faceId, type);
}

static void undoMove(void* state, int moveId) {
	int newMoveId;
	if (moveId < 6) newMoveId = moveId + 12;
	else if (moveId >= 12) newMoveId = moveId - 12;
	else moveId = moveId;
	makeMove(state, newMoveId);
}

static bool isSolved(void* state) {
	return cubeIsSolved(state);
}

static void __attribute__((constructor)) registerCube() {
	registerPuzzleModule("Standard 3x3 Rubik's Cube",
		numPossibleMoves,
		&getStartState,
		&pruneState,
		&pruneMove,
		&makeMove,
		&undoMove,
		&isSolved);
}