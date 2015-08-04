#ifndef CUBE_INTERNAL_H
#define CUBE_INTERNAL_H

#include <stdint.h>

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

typedef char CubeCornerRotation;
#define NO_ROT 0
#define CLOCKWISE_ROT 1
#define COUNTER_ROT 2

#define wrapCornerRotation(rot) ((rot) % 3)

typedef char CubeEdgeFlip;
#define NO_FLIP 0
#define FLIP 1

#define wrapEdgeFlip(flip) ((flip) % 2)

struct Corner {
	CubeCornerId id;
	CubeCornerRotation rotation;
};

struct Edge {
	CubeEdgeId id;
	CubeEdgeFlip flip;
};

struct Cube {
	struct Corner corners[8];
	struct Edge edges[12];
};

CubeFaceId getStickerFace(Cube cube, CubeFaceId curFaceId, int x, int y);

#endif
