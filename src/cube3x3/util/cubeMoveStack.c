#include <stdlib.h>
#include <stdbool.h>

#include <cube3x3/cubeMoveStack.h>

#include <cube3x3/cube.h>

CubeMoveStack newCubeMoveStack() {
	return NULL;
}

bool hasCubeMove(CubeMoveStack stack) {
	return (stack != NULL);
}

CubeMoveStack pushCubeMove(CubeMoveStack stack, CubeFaceId faceId, TurnType turnType) {
	struct CubeMoveStackNode* newNode = malloc(sizeof(struct CubeMoveStackNode));
	newNode->data.faceId = faceId;
	newNode->data.turnType = turnType;
	newNode->next = stack;
	return newNode;
}

CubeMoveStack popCubeMove(CubeMoveStack stack, struct CubeMoveData* move) {
	struct CubeMoveStackNode* oldNode = stack;
	*move = oldNode->data;
	stack = oldNode->next;
	free(oldNode);
	return stack;
}