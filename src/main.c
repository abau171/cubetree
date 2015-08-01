#include <stdio.h>

#include "cube/cube.h"

void main() {
	Cube cube = newCube();
	turnCubeFace(cube, U, CLOCKWISE_TURN);
	turnCubeFace(cube, U, COUNTER_TURN);
	printCube(cube);
	freeCube(cube);
}
