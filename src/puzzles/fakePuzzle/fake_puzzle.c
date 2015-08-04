#include <stdlib.h>

#include "../../puzzle_module.h"

static void __attribute__((constructor)) registerFake() {
	registerPuzzleModule("Fake Puzzle",
		0,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);
}
