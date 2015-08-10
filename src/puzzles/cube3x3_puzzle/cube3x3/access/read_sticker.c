#include "../cube.h"
#include "../model.h"
#include "../access.h"

static CubeFaceId getEdgeStickerFace(Cube cube, CubeFaceId faceId, int edgeOnFaceId) {
	return getEdgeFaceletOnFace(cube, faceId, edgeOnFaceId);
}

static CubeFaceId getCornerStickerFace(Cube cube, CubeFaceId faceId, CubeCornerOnFaceId cornerOnFaceId) {
	return getCornerFaceletOnFace(cube, faceId, cornerOnFaceId);
}

CubeFaceId getStickerFace(Cube cube, CubeFaceId faceId, int x, int y) {
	if (x == 1 && y == 1) {
		return faceId; // center sticker
	} else if (x == 1 || y == 1) {
		int edgeStickerId = (x % 2 == 1) ? (y * 3 / 2) : (1 + x / 2);
		int edgeOnFaceId = ((int[]) {U_EOF, L_EOF, R_EOF, D_EOF})[edgeStickerId];
		return getEdgeStickerFace(cube, faceId, edgeOnFaceId);
	} else {
		int cornerStickerId = (x / 2) + y;
		int cornerOnFaceId = ((int[]) {2, 3, 1, 0})[cornerStickerId];
		return getCornerStickerFace(cube, faceId, cornerOnFaceId);
	}
}
