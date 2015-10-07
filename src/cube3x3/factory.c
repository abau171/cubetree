#include <stdbool.h>
#include <stdlib.h>

#include <cube3x3/cube.h>
#include <cube3x3/model.h>

/*
 * Solved state of a cube used for initializing new cubes.
 */
struct Cube solvedCube = {
	.corners = {
		{UFR_CORNER, NO_ROT},
		{UFL_CORNER, NO_ROT},
		{UBL_CORNER, NO_ROT},
		{UBR_CORNER, NO_ROT},
		{DFR_CORNER, NO_ROT},
		{DFL_CORNER, NO_ROT},
		{DBL_CORNER, NO_ROT},
		{DBR_CORNER, NO_ROT}
	},
	.edges = {
		{UF_EDGE, NO_FLIP},
		{UL_EDGE, NO_FLIP},
		{UB_EDGE, NO_FLIP},
		{UR_EDGE, NO_FLIP},
		{FR_EDGE, NO_FLIP},
		{FL_EDGE, NO_FLIP},
		{BL_EDGE, NO_FLIP},
		{BR_EDGE, NO_FLIP},
		{DF_EDGE, NO_FLIP},
		{DL_EDGE, NO_FLIP},
		{DB_EDGE, NO_FLIP},
		{DR_EDGE, NO_FLIP}
	}
};

/*
 * Generates a new cube.
 */
Cube newCube() {
	Cube cube = malloc(sizeof(struct Cube));
	*cube = solvedCube;
	return cube;
}

/*
 * Clones a cube.
 */
Cube cloneCube(Cube originalCube) {
	Cube cube = malloc(sizeof(struct Cube));
	*cube = *originalCube;
	return cube;
}

/*
 * Frees a cube.*/
void freeCube(Cube cube) {
	free(cube);
}
