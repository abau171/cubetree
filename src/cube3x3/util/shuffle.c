#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <cube3x3/cube.h>
#include <cube3x3/model.h>

/*
 * Shuffles a cube a specific number of moves from its current state.
 */
struct CubeMoveData* shuffleCube(Cube cube, int numTurns) {
	srand(time(NULL));
	CubeFaceId lastFaceId = NO_FACE;
	struct CubeMoveData* shuffleMoves = malloc(sizeof(struct CubeMoveData) * numTurns);
	for (int i = 0; i < numTurns; i++) {
		CubeFaceId nextFaceId;
		do {
			nextFaceId = (rand() % 6);
		} while (nextFaceId == lastFaceId);
		TurnType type = (rand() % 3) + 1;
		shuffleMoves[i].faceId = nextFaceId;
		shuffleMoves[i].turnType = type;
		turnCubeFace(cube, nextFaceId, type);
		lastFaceId = nextFaceId;
	}
	return shuffleMoves;
}
