#include "../cube.h"
#include "../model.h"
#include "../access.h"

static CubeEdgeId edgeIdsByFace[6][4] = {
	{UB_EDGE, UL_EDGE, UR_EDGE, UF_EDGE},
	{UL_EDGE, BL_EDGE, FL_EDGE, DL_EDGE},
	{UF_EDGE, FL_EDGE, FR_EDGE, DF_EDGE},
	{UR_EDGE, FR_EDGE, BR_EDGE, DR_EDGE},
	{UB_EDGE, BR_EDGE, BL_EDGE, DB_EDGE},
	{DF_EDGE, DL_EDGE, DR_EDGE, DB_EDGE}
};

static CubeEdgeFlip flipsByFace[6][4] = {
	{NO_FLIP, NO_FLIP, NO_FLIP, NO_FLIP},
	{FLIP, FLIP, FLIP, FLIP},
	{FLIP, NO_FLIP, NO_FLIP, FLIP},
	{FLIP, FLIP, FLIP, FLIP},
	{FLIP, NO_FLIP, NO_FLIP, FLIP},
	{NO_FLIP, NO_FLIP, NO_FLIP, NO_FLIP}
};

static CubeFaceId faceByEdge[12][2] = {
	{U_FACE, F_FACE},
	{U_FACE, L_FACE},
	{U_FACE, B_FACE},
	{U_FACE, R_FACE},
	{F_FACE, R_FACE},
	{F_FACE, L_FACE},
	{B_FACE, L_FACE},
	{B_FACE, R_FACE},
	{D_FACE, F_FACE},
	{D_FACE, L_FACE},
	{D_FACE, B_FACE},
	{D_FACE, R_FACE}
};

static CubeFaceId getEdgeStickerFace(Cube cube, CubeFaceId faceId, int edgeStickerId) {
	CubeEdgeId slotEdgeId = edgeIdsByFace[faceId][edgeStickerId];
	CubeEdgeId fillEdgeId = cube->edges[slotEdgeId].id;
	CubeEdgeFlip slotFlip = flipsByFace[faceId][edgeStickerId];
	CubeEdgeFlip fillFlip = wrapEdgeFlip(2 + slotFlip - cube->edges[slotEdgeId].flip);
	return faceByEdge[fillEdgeId][fillFlip];
}

static CubeFaceId getCornerStickerFace(Cube cube, CubeFaceId faceId, CubeCornerOnFaceId cornerOnFaceId) {
	return getCornerFaceletOnFace(cube, faceId, cornerOnFaceId);
}

CubeFaceId getStickerFace(Cube cube, CubeFaceId faceId, int x, int y) {
	if (x == 1 && y == 1) {
		return faceId; // center sticker
	} else if (x == 1 || y == 1) {
		int edgeStickerId = (x % 2 == 1) ? (y * 3 / 2) : (1 + x / 2);
		return getEdgeStickerFace(cube, faceId, edgeStickerId);
	} else {
		int cornerStickerId = (x / 2) + y;
		int cornerOnFaceId = ((int[]) {2, 3, 1, 0})[cornerStickerId];
		return getCornerStickerFace(cube, faceId, cornerOnFaceId);
	}
}
