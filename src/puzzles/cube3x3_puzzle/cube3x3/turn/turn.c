#include <stdbool.h>

#include "../cube.h"
#include "../model.h"
#include "turn_internal.h"

/*
 * Turns a face of a cube 90 degrees in either a clockwise or counter-clockwise direction.
 */
/*static void quarterTurnFace(Cube cube, CubeFaceId faceId, bool clockwise) {
	quarterRotateCorners(cube, faceId, type);
	quarterRevolveCorners(cube, faceId, clockwise);
	quarterFlipEdges(cube, faceId, type);
	quarterRevolveEdges(cube, faceId, type);
}*/

/*
 * Turns a face of a cube in a given direction exactly as a real cube would be turned.
 */
void turnCubeFace(Cube cube, CubeFaceId faceId, TurnType type) {
	if (type == CLOCKWISE_TURN || type == COUNTER_TURN) {
		bool clockwise = (type == CLOCKWISE_TURN);
		quarterRotateCorners(cube, faceId, type);
		quarterRevolveCorners(cube, faceId, clockwise);
		quarterFlipEdges(cube, faceId, type);
		quarterRevolveEdges(cube, faceId, type);
	} else if (type == DOUBLE_TURN) {
		turnCubeFace(cube, faceId, CLOCKWISE_TURN);
		turnCubeFace(cube, faceId, CLOCKWISE_TURN);
	}
}
