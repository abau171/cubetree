#include <stdio.h>

#include "../cube.h"
#include "../model.h"
#include "../access.h"

#define NUM_MIDDLE_FACES 4

/*
 * Lookup table mapping faces of a cube to characters representing the sticker color of that face.
 */
static char stickers[NUM_CUBE_FACES] = {'B', 'O', 'W', 'R', 'Y', 'G'};

/*
 * List of the middle faces of a cube used for printing in order
 */
static CubeFaceId middleFaces[NUM_MIDDLE_FACES] = {L_FACE, F_FACE, R_FACE, B_FACE};

/*
 * Prints the character representing the sticker color of a face of a cube.
 */
static void printFaceStickerColor(CubeFaceId faceId) {
	putchar(stickers[faceId]);
}

/*
 * Prints a horizontal row of stickers on a face of a cube.
 */
static void printStickerRow(Cube cube, CubeFaceId faceId, int y) {
	for (int x = 0; x < DIM_CUBE; x++) {
		printFaceStickerColor(getFacelet(cube, faceId, x, y));
		if (x < (DIM_CUBE - 1)) putchar(' ');
	}
}

/*
 * Overwrites the existing sticker characters used for printing.
 * 
 * Takes an array of 6 characters in the order:
 *     UP, LEFT, FRONT, RIGHT, BACK, DOWN
 */
void setStickerChars(char* newStickers) {
	for (int i = 0; i < NUM_CUBE_FACES; i++) {
		stickers[i] = newStickers[i];
	}
}

/*
 * Prints a cube.
 */
void printCube(Cube cube) {
	for (int y = 0; y < DIM_CUBE; y++) {
		printf("     ");
		printStickerRow(cube, U_FACE, y);
		putchar('\n');
	}
	for (int y = 0; y < DIM_CUBE; y++) {
		for (int i = 0; i < NUM_MIDDLE_FACES; i++) {
			CubeFaceId faceId = middleFaces[i];
			printStickerRow(cube, faceId, y);
		}
		putchar('\n');
	}
	for (int y = 0; y < DIM_CUBE; y++) {
		printf("     ");
		printStickerRow(cube, D_FACE, y);
		putchar('\n');
	}
}