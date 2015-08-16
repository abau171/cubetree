#include <stdbool.h>

#include "../../puzzle_module.h"
#include "cube3x3_puzzle.h"
#include "cube3x3/cube.h"

#define SHUFFLE_TURNS 5

#define NUM_POSSIBLE_MOVES 18

static struct Move getMove(int moveId) {
	struct Move move;
	move.faceId = (moveId % 6);
	move.type = (moveId - move.faceId) / 6 + 1;
	return move;
}

static void* getStartState() {
	Cube cube = newCube();
	shuffleCube(cube, SHUFFLE_TURNS);
	printCube(cube);
	return cube;
}

static bool pruneState(void* state) {
	(void) state;
	return false;
}

static bool pruneMove(void* state, int moveId) {
	(void) state;
	(void) moveId;
	return false;
}

static void makeMove(void* state, int moveId) {
	struct Move move = getMove(moveId);
	turnCubeFace(state, move.faceId, move.type);
}

static void undoMove(void* state, int moveId) {
	int newMoveId;
	if (moveId < 6) newMoveId = moveId + 12;
	else if (moveId >= 12) newMoveId = moveId - 12;
	else newMoveId = moveId;
	makeMove(state, newMoveId);
}

static bool isSolved(void* state) {
	return cubeIsSolved(state);
}

static void printMove(int moveId) {
	struct Move move = getMove(moveId);
	printTurn(move.faceId, move.type, true);
}

static void __attribute__((constructor)) registerPuzzle() {
	struct PuzzleModule puzzleModule = makePuzzleModule(
		"Standard 3x3 Rubik's Cube",
		NUM_POSSIBLE_MOVES,
		&getStartState,
		&pruneState,
		&pruneMove,
		&makeMove,
		&undoMove,
		&isSolved,
		&printMove
	);
	registerPuzzleModule(puzzleModule);
}
