#include <cube3x3/cube.h>
#include <cube3x3/model.h>
#include <cube3x3/access.h>

/*
 * Lookup table mapping face and edge-on-face pairs to edge slots.
 */
static struct Edge edgeSlotsOnFace[NUM_CUBE_FACES][NUM_CUBE_EDGES_PER_FACE] = {
	{ {UF_EDGE, NO_FLIP}, {UL_EDGE, NO_FLIP}, {UB_EDGE, NO_FLIP}, {UR_EDGE, NO_FLIP} },
	{ {DL_EDGE, FLIP}, {BL_EDGE, FLIP}, {UL_EDGE, FLIP}, {FL_EDGE, FLIP} },
	{ {DF_EDGE, FLIP}, {FL_EDGE, NO_FLIP}, {UF_EDGE, FLIP}, {FR_EDGE, NO_FLIP} },
	{ {DR_EDGE, FLIP}, {FR_EDGE, FLIP}, {UR_EDGE, FLIP}, {BR_EDGE, FLIP} },
	{ {DB_EDGE, FLIP}, {BR_EDGE, NO_FLIP}, {UB_EDGE, FLIP}, {BL_EDGE, NO_FLIP} },
	{ {DB_EDGE, NO_FLIP}, {DL_EDGE, NO_FLIP}, {DF_EDGE, NO_FLIP}, {DR_EDGE, NO_FLIP} }
};

/*
 * Lookup table mapping edge ids and slot flips to the facelet at that flip on the edge piece.
 */
static CubeFaceId edgeFacelets[NUM_CUBE_EDGES][NUM_CUBE_FLIPS_PER_EDGE] = {
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

/*
 * Gets the edge slot on a face of a cube.
 * 
 * Edge slots are flipped so that applying their flip to an unflipped edge in that slot will align the facelet belonging to the given face with the dominant face.
 */
struct Edge getEdgeSlotOnFace(CubeFaceId faceId, CubeEdgeOnFaceId edgeOnFaceId) {
	return edgeSlotsOnFace[faceId][edgeOnFaceId];
}

/*
 * Gets the facelet at the given flip of an edge slot in a cube.
 * 
 * The facelet found is the one where applying the given flip to the unflipped edge aligns the facelet with the dominant face.
 */
CubeFaceId getFaceletOnEdge(CubeEdgeId edgeId, CubeEdgeFlip flip) {
	return edgeFacelets[edgeId][flip];
}

/*
 * Gets the edge on a face of a cube.
 * 
 * The edge's flip is changed so that the given face is considered the dominant face, but the edge's dominant facelet remains the same.
 */
struct Edge getEdgeOnFace(Cube cube, CubeFaceId faceId, CubeEdgeOnFaceId edgeOnFaceId) {
	struct Edge edgeSlot = getEdgeSlotOnFace(faceId, edgeOnFaceId);
	struct Edge edge = getEdge(cube, edgeSlot.id);
	struct Edge edgeOnFace = getFlippedEdge(edge, edgeSlot.flip);
	return edgeOnFace;
}

/*
 Gets the facelet at an edge position on a face.
 */
CubeFaceId getEdgeFaceletOnFace(Cube cube, CubeFaceId faceId, CubeEdgeOnFaceId edgeOnFaceId) {
	struct Edge edgeOnFace = getEdgeOnFace(cube, faceId, edgeOnFaceId);
	CubeFaceId faceletId = getFaceletOnEdge(edgeOnFace.id, edgeOnFace.flip);
	return faceletId;
}