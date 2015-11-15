#include <stdio.h>

#include <edgeLookup2.h>

#include <lookup.h>
#include <cube3x3/cube.h>
#include <cube3x3/access.h>

// (12! / 6!) * (2 ^ 6)
#define NUM_STATES 42577920

static char edgeLookup[NUM_STATES];

void initEdgeLookup2() {
	if (loadLookupCache("edge2.cache", edgeLookup, NUM_STATES)) {
		puts("loaded edge2 cache!");
	} else {
		puts("edge2 cache not loaded!");
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

	// calculate for edge flips
	for (int i = 6; i < 12; i++) {
		struct Edge curEdge = getEdge(cube, i);
		key *= 2;
		key += curEdge.flip;
	}
	return key;
}

char lookupEdge2Distance(Cube cube) {
	return edgeLookup[encodeEdges2(cube)];
}