#include <stdlib.h>
#include <string.h>

#include "puzzle_module.h"

PuzzleModuleNode firstPuzzleModuleNode = NULL;

void registerPuzzleModule(char* moduleName,
		int numPossibleMoves,
		StartStateGetter getStartState,
		StatePruner pruneState,
		MovePruner pruneMove,
		MoveMaker makeMove,
		MoveUndoer undoMove,
		SolveChecker isSolved) {
	struct PuzzleModule newMod;
	char* newModuleName = malloc(strlen(moduleName));
	strcpy(newModuleName, moduleName);
	newMod.moduleName = newModuleName;
	newMod.numPossibleMoves = numPossibleMoves;
	newMod.getStartState = getStartState;
	newMod.pruneState = pruneState;
	newMod.pruneMove = pruneMove;
	newMod.makeMove = makeMove;
	newMod.undoMove = undoMove;
	newMod.isSolved = isSolved;
	PuzzleModuleNode newNode = malloc(sizeof(struct PuzzleModuleNode));
	newNode->puzzleModule = newMod;
	newNode->next = firstPuzzleModuleNode;
	firstPuzzleModuleNode = newNode;
}