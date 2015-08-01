#include <stdbool.h>
#include <stdlib.h>

#include "cube.h"
#include "cube_internal.h"

struct Cube solvedCube = {{{UFR, 0}, {UFL, 0}, {UBL, 0}, {UBR, 0}, {DFR, 0}, {DFL, 0}, {DBL, 0}, {DBR, 0}}, {{UF, 0}, {UL, 0}, {UB, 0}, {UR, 0}, {FR, 0}, {FL, 0}, {BL, 0}, {BR, 0}, {DF, 0}, {DL, 0}, {DB, 0}, {DR, 0}}};

struct Cube upCube = {{{UBR, 0}, {UFR, 0}, {UFL, 0}, {UBL, 0}, {DFR, 0}, {DFL, 0}, {DBL, 0}, {DBR, 0}}, {{UR, 0}, {UF, 0}, {UL, 0}, {UB, 0}, {FR, 0}, {FL, 0}, {BL, 0}, {BR, 0}, {DF, 0}, {DL, 0}, {DB, 0}, {DR, 0}}};

struct Cube rightCube = {{{DFR, COUNTER_ROT}, {UFL, 0}, {UBL, 0}, {UFR, CLOCKWISE_ROT}, {DBR, CLOCKWISE_ROT}, {DFL, 0}, {DBL, 0}, {UBR, COUNTER_ROT}}, {{UF, 0}, {UL, 0}, {UB, 0}, {FR, 0}, {DR, 0}, {FL, 0}, {BL, 0}, {UR, 0}, {DF, 0}, {DL, 0}, {DB, 0}, {BR, 0}}};

struct Cube frontCube = {{{UFL, CLOCKWISE_ROT}, {DFL, COUNTER_ROT}, {UBL, 0}, {UBR, 0}, {UFR, COUNTER_ROT}, {DFR, CLOCKWISE_ROT}, {DBL, 0}, {DBR, 0}}, {{FL, FLIP}, {UL, 0}, {UB, 0}, {UR, 0}, {UF, FLIP}, {DF, FLIP}, {BL, 0}, {BR, 0}, {FR, FLIP}, {DL, 0}, {DB, 0}, {DR, 0}}};

struct Cube FRCube = {{{UFR, 1}, {DFL, 2}, {UBL, 0}, {UFL, 2}, {DBR, 1}, {DFR, 1}, {DBL, 0}, {UBR, 2}}, {{FL, 1}, {UL, 0}, {UB, 0}, {UF, 1}, {DR, 0}, {DF, 1}, {BL, 0}, {UR, 0}, {FR, 1}, {DL, 0}, {DB, 0}, {BR, 0}}};

struct Cube boringCube = {{{UFR, 0}, {UFR, 0}, {UFR, 0}, {UFR, 0}, {UFR, 0}, {UFR, 0}, {UFR, 0}, {UFR, 0}}, {{UF, 0}, {UF, 0}, {UF, 0}, {UF, 0}, {UF, 0}, {UF, 0}, {UF, 0}, {UF, 0}, {UF, 0}, {UF, 0}, {UF, 0}, {UF, 0}}};

Cube newCube() {
	Cube cube = malloc(sizeof(struct Cube));
	*cube = solvedCube;
	return cube;
}

void freeCube(Cube cube) {
	free(cube);
}
