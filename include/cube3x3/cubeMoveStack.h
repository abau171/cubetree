#ifndef CUBE_LIST_H
#define CUBE_LIST_H

#include <cube3x3/cube.h>

struct CubeMoveStackNode {
	struct CubeMoveData data;
	struct CubeMoveStackNode* next;
};

typedef struct CubeMoveStackNode* CubeMoveStack;

CubeMoveStack newCubeMoveStack();

bool hasCubeMove(CubeMoveStack stack);

CubeMoveStack pushCubeMove(CubeMoveStack stack, CubeFaceId faceId, TurnType turnType);

CubeMoveStack popCubeMove(CubeMoveStack stack, struct CubeMoveData* move);

#endif