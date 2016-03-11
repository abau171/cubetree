#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <cube_utils.h>
#include <cube.h>
#include <ida_star.h>
#include <corner_lookup.h>
#include <upper_edge_lookup.h>
#include <lower_edge_lookup.h>

static bool prune_state(const cube_t* cube, int depth) {
	int cornersystem_encoding = encodeCornerSystem(&cube->cornersystem);
	if (lookupCornerDistance(cornersystem_encoding) > depth) {
		return true;
	}
	int upper_edgesystem_encoding = encodeUpperEdgeSystem(&cube->edgesystem);
	if (lookupUpperEdgeDistance(upper_edgesystem_encoding) > depth) {
		return true;
	}
	int lower_edgesystem_encoding = encodeLowerEdgeSystem(&cube->edgesystem);
	if (lookupLowerEdgeDistance(lower_edgesystem_encoding) > depth) {
		return true;
	}
	return false;
}

static uint8_t opposite_faces[7] = {D_FACE, R_FACE, B_FACE, L_FACE, F_FACE, U_FACE, 6};

static bool searchDepth(const cube_t* last_cube, int depth, uint8_t last_face) {
	if (depth == 0) {
		return isSolvedCube(last_cube);
	} else if (prune_state(last_cube, depth)) {
		return false;
	} else {
		cube_t cur_cube;
		for (uint8_t face = 0; face < 6; face++) {
			if (face == last_face) {
				continue;
			} else if (face < 3 && face == opposite_faces[last_face]) {
				continue;
			}
			for (int turn_type = 1; turn_type < 4; turn_type++) {
				turnCube(&cur_cube, last_cube, face, turn_type);
				bool result = searchDepth(&cur_cube, depth - 1, face);
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
		bool result = searchDepth(cube, depth, 6);
		if (result) {
			return result;
		}
		depth++;
	}
}
