#include <stdlib.h>
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

movenode_t* searchDepth(const cube_t* last_cube, int depth, uint8_t last_face) {
	cube_t cur_cube;
	for (uint8_t face = 0; face < 6; face++) {
		if (face == last_face) {
			continue;
		} else if (face < 3 && face == opposite_faces[last_face]) {
			continue;
		}
		for (int turn_type = 1; turn_type < 4; turn_type++) {
			if (depth >= 15) {
				for (int i = 0; i < depth - 15; i++) {
					putchar('\t');
				}
				printf("Face: %d Turn: %d\n", face, turn_type);
			}
			turnCube(&cur_cube, last_cube, face, turn_type);
			if (depth - 1 == 0) {
				if (isSolvedCube(&cur_cube)) {
					movenode_t* solution = malloc(sizeof(movenode_t));
					solution->face = face;
					solution->turn_type = turn_type;
					solution->next_node = NULL;
					return solution;
				} else {
					continue;
				}
			} else if (prune_state(&cur_cube, depth - 1)) {
				continue;
			} else {
				movenode_t* partial_solution = searchDepth(&cur_cube, depth - 1, face);
				if (partial_solution != NULL) {
					movenode_t* solution = malloc(sizeof(movenode_t));
					solution->face = face;
					solution->turn_type = turn_type;
					solution->next_node = partial_solution;
					return solution;
				}
			}
		}
	}
	return NULL;
}

movenode_t* idaStar(const cube_t* cube) {
	int depth = 0;
	while (true) {
		printf("DEPTH %d\n", depth);
		movenode_t* result = searchDepth(cube, depth, 6);
		if (result != NULL) {
			return result;
		}
		depth++;
	}
}
