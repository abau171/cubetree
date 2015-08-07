#include <stdbool.h>
#include <stdlib.h>

#include "cube.h"
#include "cube_internal.h"

struct Cube solvedCube = {
	.corners = {
		{UFR, NO_ROT},
		{UFL, NO_ROT},
		{UBL, NO_ROT},
		{UBR, NO_ROT},
		{DFR, NO_ROT},
		{DFL, NO_ROT},
		{DBL, NO_ROT},
		{DBR, NO_ROT}
	},
	.edges = {
		{UF, NO_FLIP},
		{UL, NO_FLIP},
		{UB, NO_FLIP},
		{UR, NO_FLIP},
		{FR, NO_FLIP},
		{FL, NO_FLIP},
		{BL, NO_FLIP},
		{BR, NO_FLIP},
		{DF, NO_FLIP},
		{DL, NO_FLIP},
		{DB, NO_FLIP},
		{DR, NO_FLIP}
	}
};

Cube newCube() {
	Cube cube = malloc(sizeof(struct Cube));
	*cube = solvedCube;
	return cube;
}

void freeCube(Cube cube) {
	free(cube);
}
