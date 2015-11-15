#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include <idaStar.h>

#include <cube3x3/cube.h>
#include <cornerLookup.h>
#include <edgeLookup.h>
#include <edgeLookup2.h>

#define NUM_SHUFFLE_MOVES 12

int main() {

	initCornerLookup();
	initEdgeLookup();
	initEdgeLookup2();

	Cube cube = newCube();
	struct CubeMoveData* shuffleMoves = shuffleCube(cube, NUM_SHUFFLE_MOVES);
	for (int i = 0; i < NUM_SHUFFLE_MOVES; i++) {
		printTurn(shuffleMoves[i], true);
	}
	free(shuffleMoves);
	printCube(cube);
	solveCube(cube);
	printCube(cube);
	putchar('\n');
}