#include <stdlib.h>
#include <stdio.h>

#include "cube3x3/cube.h"
#include "cube3x3/access.h"

// 8! * (3 ^ 7)
#define NUM_STATES 88179840

char cornerLookup[NUM_STATES];

struct DistanceCube {
	Cube cube;
	char distance;
};

struct GenNode {
	Cube cube;
	char distance;
	struct GenNode* next;
};

static struct GenNode* genQueue = NULL;
static struct GenNode* genQueueLast = NULL;

void queueGenQueue(Cube cube, char distance) {
	struct GenNode* newNode = malloc(sizeof(struct GenNode));
	newNode->cube = cube;
	newNode->distance = distance;
	newNode->next = NULL;
	if (genQueue == NULL) {
		genQueue = newNode;
		genQueueLast = newNode;
	} else {
		genQueueLast->next = newNode;
		genQueueLast = newNode;
	}
}

struct DistanceCube popGenQueue() {
	Cube returnCube = genQueue->cube;
	char returnDistance = genQueue->distance;
	struct GenNode* oldNode = genQueue;
	genQueue = oldNode->next;
	free(oldNode);
	struct DistanceCube dCube;
	dCube.cube = returnCube;
	dCube.distance = returnDistance;
	return dCube;
}

bool genQueueEmpty() {
	return (genQueue == NULL);
}

void initCornerLookup() {
	for (int i = 0; i < NUM_STATES; i++) {
		cornerLookup[i] = -2;
	}
}

int fact[] = {1, 1, 2, 6, 24, 120, 720, 5040};

int encodeCorners(Cube cube) {
	int key = 0;
	// calculate for corner positions
	for (int i = 0; i < 7; i++) {
		int numLess = 0;
		for (int j = i + 1; j < 8; j++) {
			if (getCorner(cube, i).id > getCorner(cube, j).id) {
				numLess++;
			}
		}
		key += numLess * fact[7 - i];
	}
	// calculate for corner rotations
	for (int i = 0; i < 7; i++) {
		struct Corner curCorner = getCorner(cube, i);
		key *= 3;
		key += curCorner.rotation;
	}
	return key;
}

void genCornerLookup() {
	initCornerLookup();
	Cube initCube = newCube();
	queueGenQueue(initCube, 0);
	cornerLookup[encodeCorners(initCube)] = -1;
	struct DistanceCube curCube;
	int count = 0;
	int stackDepth = 1;
	while (!genQueueEmpty()) {
		curCube = popGenQueue();
		stackDepth--;
		int index = encodeCorners(curCube.cube);
		char distance = cornerLookup[index];
		if (distance == -1) {
			for (int i = 0; i < 18; i++) {
				Cube clone = cloneCube(curCube.cube);
				CubeFaceId faceId = i % 6;
				TurnType type = (i - faceId) / 6 + 1;
				turnCubeFace(clone, faceId, type);
				int cloneIndex = encodeCorners(clone);
				if (cornerLookup[cloneIndex] == -2) {
					cornerLookup[cloneIndex] = -1;
					queueGenQueue(clone, curCube.distance + 1);
					stackDepth++;
				} else {
					freeCube(clone);
				}
			}
			cornerLookup[index] = curCube.distance;
			count++;
			if (count % 1000000 == 0) {
				//printf("%d\t\t%d\t\t%d\n", count, stackDepth, curCube.distance);
				double percent = 100.0 * count / (double) NUM_STATES;
				printf("%9.4G%%\n", percent);
			}
		}
		freeCube(curCube.cube);
	}
}

char lookupCornerDistance(Cube cube) {
	return cornerLookup[encodeCorners(cube)];
}

void cacheCornerLookup() {
	FILE* file = fopen("corner.cache", "wb");
	fwrite(cornerLookup, sizeof(char), NUM_STATES, file);
	fclose(file);
}

void loadCornerLookup() {
	FILE* file = fopen("corner.cache", "rb");
	fread(cornerLookup, sizeof(char), NUM_STATES, file);
	fclose(file);
}