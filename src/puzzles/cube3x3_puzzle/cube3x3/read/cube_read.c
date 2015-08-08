#include "../cube.h"
#include "../model.h"

static CubeEdgeId edgeIdsByFace[6][4] = {
	{UB, UL, UR, UF},
	{UL, BL, FL, DL},
	{UF, FL, FR, DF},
	{UR, FR, BR, DR},
	{UB, BR, BL, DB},
	{DF, DL, DR, DB}};

static CubeEdgeFlip flipsByFace[6][4] = {
	{NO_FLIP, NO_FLIP, NO_FLIP, NO_FLIP},
	{FLIP, FLIP, FLIP, FLIP},
	{FLIP, NO_FLIP, NO_FLIP, FLIP},
	{FLIP, FLIP, FLIP, FLIP},
	{FLIP, NO_FLIP, NO_FLIP, FLIP},
	{NO_FLIP, NO_FLIP, NO_FLIP, NO_FLIP}};

static CubeFaceId faceByEdge[12][2] = {
	{U, F},
	{U, L},
	{U, B},
	{U, R},
	{F, R},
	{F, L},
	{B, L},
	{B, R},
	{D, F},
	{D, L},
	{D, B},
	{D, R}};

static CubeFaceId getEdgeStickerFace(Cube cube, CubeFaceId faceId, int edgeStickerId) {
	CubeEdgeId slotEdgeId = edgeIdsByFace[faceId][edgeStickerId];
	CubeEdgeId fillEdgeId = cube->edges[slotEdgeId].id;
	CubeEdgeFlip slotFlip = flipsByFace[faceId][edgeStickerId];
	CubeEdgeFlip fillFlip = wrapEdgeFlip(2 + slotFlip - cube->edges[slotEdgeId].flip);
	return faceByEdge[fillEdgeId][fillFlip];
}

static CubeCornerId cornerIdsByFace[6][4] = {
	{UBL, UBR, UFL, UFR},
	{UBL, UFL, DBL, DFL},
	{UFL, UFR, DFL, DFR},
	{UFR, UBR, DFR, DBR},
	{UBR, UBL, DBR, DBL},
	{DFL, DFR, DBL, DBR}};

static CubeCornerRotation rotsByFace[6][4] = {
	{NO_ROT, NO_ROT, NO_ROT, NO_ROT},
	{CLOCKWISE_ROT, COUNTER_ROT, COUNTER_ROT, CLOCKWISE_ROT},
	{CLOCKWISE_ROT, COUNTER_ROT, COUNTER_ROT, CLOCKWISE_ROT},
	{CLOCKWISE_ROT, COUNTER_ROT, COUNTER_ROT, CLOCKWISE_ROT},
	{CLOCKWISE_ROT, COUNTER_ROT, COUNTER_ROT, CLOCKWISE_ROT},
	{NO_ROT, NO_ROT, NO_ROT, NO_ROT}};

static CubeFaceId faceByCorner[8][3] = {
	{U, R, F},
	{U, F, L},
	{U, L, B},
	{U, B, R},
	{D, F, R},
	{D, L, F},
	{D, B, L},
	{D, R, B}};

static CubeFaceId getCornerStickerFace(Cube cube, CubeFaceId faceId, int cornerStickerId) {
	CubeCornerId slotCornerId = cornerIdsByFace[faceId][cornerStickerId];
	CubeCornerId fillCornerId = cube->corners[slotCornerId].id;
	CubeCornerRotation slotRotation = rotsByFace[faceId][cornerStickerId];
	CubeCornerRotation fillRotation = wrapCornerRotation(3 + slotRotation - cube->corners[slotCornerId].rotation);
	return faceByCorner[fillCornerId][fillRotation];
}

CubeFaceId getStickerFace(Cube cube, CubeFaceId faceId, int x, int y) {
	if (x == 1 && y == 1) {
		return faceId; // center sticker
	} else if (x == 1 || y == 1) {
		int edgeStickerId = (x % 2 == 1) ? (y * 3 / 2) : (1 + x / 2);
		return getEdgeStickerFace(cube, faceId, edgeStickerId);
	} else {
		int cornerStickerId = (x / 2) + y;
		return getCornerStickerFace(cube, faceId, cornerStickerId);
	}
}
