#include <stdio.h>
#include <stdbool.h>

#include <cube3x3/cube.h>
#include <cube3x3/model.h>

/*
 * Lookup table mapping faces of a cube to characters representing that face.
 */
static char faceChars[NUM_CUBE_FACES] = {'U', 'L', 'F', 'R', 'B', 'D'};

/*
 * Character representing a turn's inversion.
 */
static char inversionChar = '\'';

/*
 * Prints the character representing a face.
 */
static char printFaceChar(CubeFaceId faceId) {
	char faceChar = faceChars[faceId];
	return putchar(faceChar);
}

/*
 * Prints the character representing a turn inversion.
 */
static char printInversionChar() {
	return putchar(inversionChar);
}

/*
 * Prints the string representation of a cube turn.
 */
void printTurn(CubeFaceId faceId, TurnType type, bool includeNewline) {
	printFaceChar(faceId);
	if (type == COUNTER_TURN) {
		printInversionChar();
	} else if (type == DOUBLE_TURN) {
		printFaceChar(faceId);
	}
	if (includeNewline) {
		putchar('\n');
	}
}
