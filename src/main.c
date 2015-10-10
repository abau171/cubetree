#include <stdio.h>
#include <unistd.h>

#include <idaStar.h>

#include <cube3x3/cube.h>

int main() {

	Cube cube = newCube();
	shuffleCube(cube, 5);
	printCube(cube);
	solveCube(cube);
	printCube(cube);
	putchar('\n');
}