#include <stdlib.h>
#include <stdio.h>

#include <edgeLookup2.h>

#include <cube3x3/cube.h>
#include <cube3x3/access.h>

// (12! / 6!) * (2 ^ 6)
#define NUM_STATES 42577920

static char edge2Lookup[NUM_STATES];

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

static void queueGenQueue(Cube cube, char distance) {
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

static struct DistanceCube popGenQueue() {
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

static bool genQueueEmpty() {
	return (genQueue == NULL);
}

static void initEdge2Lookup() {
	for (int i = 0; i < NUM_STATES; i++) {
		edge2Lookup[i] = -2;
	}
}

static int fact[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};

static int ncr(int n, int r) {
	return fact[n] / (fact[r] * fact[n - r]);
}

static int encodeEdges2(Cube cube) {
	int key = 0;
	// calculate for edge positions

	int comb[6];
	int combI = 0;
	for (int i = 0; i < 12; i++) {
		CubeEdgeId curId = getEdge(cube, i).id;
		if (curId >= 6) {
			comb[combI] = i + 1;
			combI++;
		}
	}
	// combo code
	int j = 0;
	for (int i = 0; i < 6; i++) {
		for (j++; j < comb[i]; j++) {
			key += ncr(12 - j, 6 - i - 1);
		}
	}

	key *= 720; // space for perm

	CubeEdgeId perm[6];
	int permI = 0;
	for (int i = 0; i < 12; i++) {
		CubeEdgeId curId = getEdge(cube, i).id;
		if (curId >= 6) {
			perm[permI] = curId - 6;
			permI++;
		}
	}
	for (int i = 0; i < 5; i++) {
		int numLess = 0;
		for (int j = i + 1; j < 6; j++) {
			if (perm[i] > perm[j]) {
				numLess++;
			}
		}
		key += numLess * fact[5 - i];
	}



	//printf("IKEY %d\n", key);
	// calculate for edge flips
	for (int i = 6; i < 12; i++) {
		struct Edge curEdge = getEdge(cube, i);
		key *= 2;
		key += curEdge.flip;
	}
	if (key >= NUM_STATES - 1) {
		printf("KEY %d\n", key);
	}
	return key;
}

void genEdge2Lookup() {
	initEdge2Lookup();
	Cube initCube = newCube();
	queueGenQueue(initCube, 0);
	edge2Lookup[encodeEdges2(initCube)] = -1;
	struct DistanceCube curCube;
	int count = 0;
	int stackDepth = 1;
	while (!genQueueEmpty()) {
		curCube = popGenQueue();
		stackDepth--;
		int index = encodeEdges2(curCube.cube);
		char distance = edge2Lookup[index];
		if (distance == -1) {
			for (int i = 0; i < 18; i++) {
				Cube clone = cloneCube(curCube.cube);
				CubeFaceId faceId = i % 6;
				TurnType type = (i - faceId) / 6 + 1;
				turnCubeFace(clone, faceId, type);
				int cloneIndex = encodeEdges2(clone);
				if (edge2Lookup[cloneIndex] == -2) {
					edge2Lookup[cloneIndex] = -1;
					queueGenQueue(clone, curCube.distance + 1);
					stackDepth++;
				} else {
					freeCube(clone);
				}
			}
			edge2Lookup[index] = curCube.distance;
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

char lookupEdge2Distance(Cube cube) {
	return edge2Lookup[encodeEdges2(cube)];
}

void cacheEdge2Lookup() {
	FILE* file = fopen("edge2.cache", "wb");
	fwrite(edge2Lookup, sizeof(char), NUM_STATES, file);
	fclose(file);
}

void loadEdge2Lookup() {
	FILE* file = fopen("edge2.cache", "rb");
	fread(edge2Lookup, sizeof(char), NUM_STATES, file);
	fclose(file);
}