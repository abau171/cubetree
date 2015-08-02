#include <stdio.h>

#include "cube/model/cube.h"

void main() {
	Cube cube = newCube();
	shuffleCube(cube, 5);
	printCube(cube);
	freeCube(cube);
}
