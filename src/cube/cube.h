#ifndef CUBE_H
#define CUBE_H

#include <stdbool.h>

typedef char CubeCornerId;
#define UFR 0
#define UFL 1
#define UBL 2
#define UBR 3
#define DFR 4
#define DFL 5
#define DBL 6
#define DBR 7

typedef char CubeEdgeId;
#define UF 0
#define UL 1
#define UB 2
#define UR 3
#define FR 4
#define FL 5
#define BL 6
#define BR 7
#define DF 8
#define DL 9
#define DB 10
#define DR 11

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
