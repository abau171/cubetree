#ifndef CUBE_H
#define CUBE_H

#include <stdint.h>
#include <stdbool.h>

#define DIM_CUBE 3
#define NUM_CUBE_FACES 6
#define NUM_CUBE_CORNERS 8
#define NUM_CUBE_EDGES 12

typedef int8_t CubeFaceId;
enum CubeFaceId { U_FACE, L_FACE, F_FACE, R_FACE, B_FACE, D_FACE, NO_FACE = -1 };

typedef int8_t TurnType;
enum TurnType { NO_TURN, CLOCKWISE_TURN, DOUBLE_TURN, COUNTER_TURN };

typedef struct Cube* Cube;

Cube newCube();

void freeCube(Cube cube);

bool cubeIsSolved(Cube cube);

void shuffleCube(Cube cube, int numTurns);

void turnCubeFace(Cube cube, CubeFaceId face, TurnType type);

void printCube(Cube cube);

void printMove(CubeFaceId faceId, TurnType type);

CubeFaceId getStickerFace(Cube cube, CubeFaceId curFaceId, int x, int y);

#endif
