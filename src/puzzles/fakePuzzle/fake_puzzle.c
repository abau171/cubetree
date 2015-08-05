#include <stdlib.h>

#include "../../puzzle_module.h"

void turnCubeFace() {
	int i = 0;
	i++;
}

static void* getStartState() {
	return NULL;
}

static void __attribute__((constructor)) registerPuzzle() {
	registerPuzzleModule("Fake Puzzle",
		0,
		&getStartState,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);
}
