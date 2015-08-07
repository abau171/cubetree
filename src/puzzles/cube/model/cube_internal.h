#ifndef CUBE_INTERNAL_H
#define CUBE_INTERNAL_H

#include <stdint.h>

typedef int8_t CubeCornerId;
enum CubeCornerId { UFR, UFL, UBL, UBR, DFR, DFL, DBL, DBR };

typedef int8_t CubeEdgeId;
enum CubeEdgeId { UF, UL, UB, UR, FR, FL, BL, BR, DF, DL, DB, DR };

typedef int8_t CubeCornerRotation;
enum CubeCornerRotation { NO_ROT, CLOCKWISE_ROT, COUNTER_ROT };

#define wrapCornerRotation(rot) ((rot) % 3)

typedef int8_t CubeEdgeFlip;
enum CubeEdgeFlip { NO_FLIP, FLIP };

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
