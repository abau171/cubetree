#ifndef PUZZLE_MODULE_H
#define PUZZLE_MODULE_H

#include <stdbool.h>

typedef void* (*StartStateGetter)();
typedef bool (*StatePruner)(void* state);
typedef bool (*MovePruner)(void* state, int moveId);
typedef void (*MoveMaker)(void* state, int moveId);
typedef void (*MoveUndoer)(void* state, int moveId);
typedef bool (*SolveChecker)(void* state);

struct PuzzleModule {
	char* moduleName;
	int numPossibleMoves;
	StartStateGetter getStartState;
	StatePruner pruneState;
	MovePruner pruneMove;
	MoveMaker makeMove;
	MoveUndoer undoMove;
	SolveChecker isSolved;
};

typedef struct PuzzleModuleNode {
	struct PuzzleModule puzzleModule;
	struct PuzzleModuleNode* next;
}* PuzzleModuleNode;

extern PuzzleModuleNode firstPuzzleModuleNode;

void registerPuzzleModule(char* moduleName, int numPossibleMoves, StartStateGetter getStartState, StatePruner pruneState, MovePruner pruneMove, MoveMaker makeMove, MoveUndoer undoMove, SolveChecker isSolved);

#endif
