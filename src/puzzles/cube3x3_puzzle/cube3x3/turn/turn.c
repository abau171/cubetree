#include "../cube.h"
#include "../model.h"
#include "turn_internal.h"

void turnCubeFace(Cube cube, CubeFaceId faceId, TurnType type) {
	bool clockwise = (type == CLOCKWISE_TURN);
	switch (type) {
	case DOUBLE_TURN:
		turnCubeFace(cube, faceId, CLOCKWISE_TURN);
		turnCubeFace(cube, faceId, CLOCKWISE_TURN);
		break;
	case CLOCKWISE_TURN:
	case COUNTER_TURN:
		quarterRotateCorners(cube, faceId, type);
		quarterRevolveCorners(cube, faceId, clockwise);
		quarterFlipEdges(cube, faceId, type);
		quarterRevolveEdges(cube, faceId, type);
		break;
	default:
		break;
	}
}
