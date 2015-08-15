#include <stdlib.h>
#include <string.h>

#include "puzzle_module.h"

PuzzleModuleNode firstPuzzleModuleNode = NULL;

void registerPuzzleModule(struct PuzzleModule puzzleModule) {
	PuzzleModuleNode newNode = malloc(sizeof(struct PuzzleModuleNode));
	newNode->puzzleModule = puzzleModule;
	newNode->next = firstPuzzleModuleNode;
	firstPuzzleModuleNode = newNode;
}

struct PuzzleModule makePuzzleModule(
		char* moduleName,
		int numPossibleMoves,
		StartStateGetter getStartState,
		StatePruner pruneState,
		MovePruner pruneMove,
		MoveMaker makeMove,
		MoveUndoer undoMove,
		SolveChecker isSolved) {

	char* moduleNameCopy = malloc(strlen(moduleName) + sizeof(char));
	strcpy(moduleNameCopy, moduleName);

	return (struct PuzzleModule) {
		moduleNameCopy,
		numPossibleMoves,
		getStartState,
		pruneState,
		pruneMove,
		makeMove,
		undoMove,
		isSolved
	};
}