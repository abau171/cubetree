#include <stdbool.h>

#include "../cube.h"
#include "../model.h"

/*
 * Checks whether a cube is solved or not.
 * 
 * Returns true on solved, false otherwise.
 */
bool cubeIsSolved(Cube cube) {
	for (int cornerId = 0; cornerId < 8; cornerId++) {
		if (cube->corners[cornerId].id != cornerId) return false;
		if (cube->corners[cornerId].rotation != 0) return false;
	}
	return true;
}