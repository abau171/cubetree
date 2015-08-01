#ifndef CUBE_H
#define CUBE_H

#include <stdbool.h>

typedef char CubeFaceId;
#define U 0
#define L 1
#define F 2
#define R 3
#define B 4
#define D 5

typedef char TurnType;
#define NO_TURN 0
#define CLOCKWISE_TURN 1
#define DOUBLE_TURN 2
#define COUNTER_TURN 3

typedef struct Cube* Cube;

Cube newCube();

void freeCube(Cube cube);

bool isSolved(Cube cube);

void turnCubeFace(Cube cube, CubeFaceId face, TurnType type);

void printCube(Cube cube);

#endif
