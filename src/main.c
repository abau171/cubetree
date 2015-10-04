#include <stdio.h>

#include "cube3x3/cube.h"
#include "idaStar.h"

int main() {
	Cube cube = newCube();
	shuffleCube(cube, 5);
	printCube(cube);
	solveCube(cube);
	return 0;
}