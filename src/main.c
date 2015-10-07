#include <stdio.h>
#include <unistd.h>

#include <idaStar.h>
#include <lookup.h>
#include <cornerLookup.h>

#include <cube3x3/cube.h>

int main() {
	initLookups();

	for (int i = 0; i < 0; i++) {
		Cube cube = newCube();
		shuffleCube(cube, 100);
		printf("CORNER DISTANCE: %d\n", lookupCornerDistance(cube));
		freeCube(cube);
		usleep(1000000);
	}

	putchar('\n');
	puts("starting real solve!");
	putchar('\n');

	while (1) {
		Cube cube = newCube();
		shuffleCube(cube, 14);
		printCube(cube);
		solveCube(cube);
		printCube(cube);
		putchar('\n');
		break;
	}
	return 0;
}