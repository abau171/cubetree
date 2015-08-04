#include <stdio.h>

#include "../puzzle_module.h"

void main() {
	puts("Available Puzzles:");
	PuzzleModuleNode curNode = firstPuzzleModuleNode;
	int nodeNum = 0;
	while (curNode != NULL) {
		printf("\t[%d] %s\n", ++nodeNum, curNode->puzzleModule.moduleName);
		curNode = curNode->next;
	}
}
