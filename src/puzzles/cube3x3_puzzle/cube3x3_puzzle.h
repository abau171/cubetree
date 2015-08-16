#ifndef CUBE3X3_PUZZLE_H
#define CUBE3X3_PUZZLE_H

#include "cube3x3/cube.h"

struct Move {
	CubeFaceId faceId;
	TurnType type;
};

struct CubeSolveState {
	Cube cube;
	struct Move lastMove;
};

#endif