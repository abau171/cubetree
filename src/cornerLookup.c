#include <stdio.h>

#include <cornerLookup.h>

#include <lookup.h>
#include <cube3x3/cube.h>
#include <cube3x3/access.h>

// 8! * (3 ^ 7)
#define NUM_STATES 88179840

static char cornerLookup[NUM_STATES];

void initCornerLookup() {
	if (loadLookupCache("corner.cache", cornerLookup, NUM_STATES)) {
		puts("loaded corner cache!");
	} else {
		puts("corner cache not loaded!");
	}
}

static int fact[] = {1, 1, 2, 6, 24, 120, 720, 5040};

static int encodeCorners(Cube cube) {
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

char lookupCornerDistance(Cube cube) {
	return cornerLookup[encodeCorners(cube)];
}