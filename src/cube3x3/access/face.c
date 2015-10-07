#include <cube3x3/cube.h>
#include <cube3x3/model.h>
#include <cube3x3/access.h>

/*
 * Lookup table for corner on face ids.
 */
static CubeCornerOnFaceId cornerOnFaceIds[] = { UL_COF, UR_COF, DL_COF, DR_COF };

/*
 * Looks up the corner on face id using an arbitrary indexing scheme.
 */
static CubeCornerOnFaceId lookUpCornerOnFaceId(int x, int y) {
	int lookupIndex = (x / 2) + y;
	return cornerOnFaceIds[lookupIndex];
}

/*
 * Lookup table for edge on face ids.
 */
static CubeEdgeOnFaceId edgeOnFaceIds[] = { U_EOF, L_EOF, R_EOF, D_EOF };

/*
 * Looks up the edge on face id using an arbitrary indexing scheme.
 */
static CubeEdgeOnFaceId lookUpEdgeOnFaceId(int x, int y) {
	int lookupIndex = (x % 2 == 1) ? (y * 3 / 2) : (1 + x / 2);
	return edgeOnFaceIds[lookupIndex];
}

/*
 * Gets the facelet at a position on a given face.
 * 
 * The x value starts on the left and increases to the right.
 * The y value starts on the top and increases downward.
 */
CubeFaceId getFacelet(Cube cube, CubeFaceId faceId, int x, int y) {
	if (x == 1 && y == 1) {
		return faceId; // center facelet
	} else if (x == 1 || y == 1) {
		CubeEdgeOnFaceId edgeOnFaceId = lookUpEdgeOnFaceId(x, y);
		return getEdgeFaceletOnFace(cube, faceId, edgeOnFaceId);
	} else {
		CubeCornerOnFaceId cornerOnFaceId = lookUpCornerOnFaceId(x, y);
		return getCornerFaceletOnFace(cube, faceId, cornerOnFaceId);
	}
}