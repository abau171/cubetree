#include <stdlib.h>
#include <stdio.h>

#include "../puzzle_module.h"
#include "algorithm.h"

static struct PuzzleModule selectPuzzle() {
	return firstPuzzleModuleNode->next->puzzleModule;
}

static void printAvailablePuzzles() {
	puts("Available Puzzles:");
	PuzzleModuleNode curNode = firstPuzzleModuleNode;
	int nodeNum = 0;
	while (curNode != NULL) {
		printf("\t[%d] %s\n", ++nodeNum, curNode->puzzleModule.moduleName);
		curNode = curNode->next;
	}
}

int main() {
	printAvailablePuzzles();
	struct PuzzleModule selectedModule = selectPuzzle();
	runSolverAlgorithm(selectedModule);
	return EXIT_SUCCESS;
}
