#ifndef CUBE_H
#define CUBE_H

#include <stdint.h>
#include <stdbool.h>

typedef int8_t CubeFaceId;
enum CubeFaceId { NO_FACE = -1, U, L, F, R, B, D }

typedef int8_t TurnType;
enum TurnType { NO_TURN, CLOCKWISE_TURN, DOUBLE_TURN, COUNTER_TURN }

typedef struct Cube* Cube;

Cube newCube();

void freeCube(Cube cube);

bool cubeIsSolved(Cube cube);

void shuffleCube(Cube cube, int numTurns);

void turnCubeFace(Cube cube, CubeFaceId face, TurnType type);

void printCube(Cube cube);

void printMove(CubeFaceId faceId, TurnType type);

#endif
