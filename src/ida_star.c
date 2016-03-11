#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <cube.h>
#include <corner_lookup.h>

static bool searchDepth(const cube_t* last_cube, int depth) {
	if (depth == 0) {
		return isSolvedCube(last_cube);
	} else if (lookupCornerDistance(encodeCornerSystem(&last_cube->cornersystem)) > depth) {
		return false;
	} else {
		cube_t cur_cube;
		for (uint8_t face = 0; face < 6; face++) {
			for (int turn_type = 1; turn_type < 4; turn_type++) {
				turnCube(&cur_cube, last_cube, face, turn_type);
				bool result = searchDepth(&cur_cube, depth - 1);
				if (result) {
					return true;
				}
			}
		}
		return false;
	}
}

bool idaStar(const cube_t* cube) {
	int depth = 0;
	while (true) {
		printf("DEPTH %d\n", depth);
		bool result = searchDepth(cube, depth);
		if (result) {
			return result;
		}
		depth++;
	}
}
