#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <cube.h>
#include <ida_star.h>
#include <lookup.h>

static uint8_t opposite_faces[7] = {D_FACE, R_FACE, B_FACE, L_FACE, F_FACE, U_FACE, 6};

static bool prune_move(uint8_t face, uint8_t last_face) {
	if (face == last_face) {
		return true;
	} else if (face < 3 && face == opposite_faces[last_face]) {
		return true;
	}
	return false;
}

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

movenode_t* prependMoveNode(movenode_t* move_list, uint8_t face, int turn_type) {
	movenode_t* new_move_list = malloc(sizeof(movenode_t));
	new_move_list->face = face;
	new_move_list->turn_type = turn_type;
	new_move_list->next_node = move_list;
	return new_move_list;
}

// depth >= 1
movenode_t* searchDepth(const cube_t* last_cube, int depth, uint8_t last_face, bool* cancel_flag) {
	if (*cancel_flag) return NULL;
	cube_t cur_cube;
	for (uint8_t face = 0; face < 6; face++) {
		if (prune_move(face, last_face)) continue;
		for (int turn_type = 1; turn_type < 4; turn_type++) {
			turnCube(&cur_cube, last_cube, face, turn_type);
			// if depth will be 0, check if solution found
			if (depth - 1 == 0) {
				if (isSolvedCube(&cur_cube)) {
					return prependMoveNode(NULL, face, turn_type);
				}
			// otherwise, if the state doesn't need to be pruned, search deeper
			} else if (!prune_state(&cur_cube, depth - 1)) {
				movenode_t* partial_solution = searchDepth(&cur_cube, depth - 1, face, cancel_flag);
				if (partial_solution != NULL) {
					return prependMoveNode(partial_solution, face, turn_type);
				}
			}
		}
	}
	return NULL;
}

// cube must not already be solved
movenode_t* idaStar(const cube_t* cube) {
	int depth = 0;
	bool cancel_flag = false;
	while (true) {
		depth++;
		printf("DEPTH %d\n", depth);
		movenode_t* result = searchDepth(cube, depth, 6, &cancel_flag);
		if (result != NULL) {
			return result;
		}
	}
}
