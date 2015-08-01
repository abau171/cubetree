#include <stdio.h>

#include "cube/cube.h"

void main() {
	Cube cube = newCube();
	shuffleCube(cube);
	printCube(cube);
	freeCube(cube);
}
