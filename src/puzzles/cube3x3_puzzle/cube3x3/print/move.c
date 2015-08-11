#include <stdio.h>

#include "../cube.h"
#include "../model.h"

static char faceChars[NUM_CUBE_FACES] = {'U', 'L', 'F', 'R', 'B', 'D'};

void printMove(CubeFaceId faceId, TurnType type) {
	putchar(faceChars[faceId]);
	if (type == COUNTER_TURN) {
		putchar('\'');
	} else if (type == DOUBLE_TURN) {
		putchar(faceChars[faceId]);
	}
	putchar('\n');
}
