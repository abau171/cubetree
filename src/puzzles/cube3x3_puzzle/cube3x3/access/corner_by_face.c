#include "../cube.h"
#include "../model.h"
#include "../access.h"

/*
 * Lookup table mapping face and corner-on-face pairs to corner slots.
 */
static struct Corner cornerSlotsOnFace[NUM_CUBE_FACES][NUM_CUBE_CORNERS_PER_FACE] = {
	{ {UFR_CORNER, NO_ROT}, {UFL_CORNER, NO_ROT}, {UBL_CORNER, NO_ROT}, {UBR_CORNER, NO_ROT} },
	{ {DFL_CORNER, COUNTER_ROT}, {DBL_CORNER, CLOCKWISE_ROT}, {UBL_CORNER, COUNTER_ROT}, {UFL_CORNER, CLOCKWISE_ROT} },
	{ {DFR_CORNER, COUNTER_ROT}, {DFL_CORNER, CLOCKWISE_ROT}, {UFL_CORNER, COUNTER_ROT}, {UFR_CORNER, CLOCKWISE_ROT} },
	{ {DBR_CORNER, COUNTER_ROT}, {DFR_CORNER, CLOCKWISE_ROT}, {UFR_CORNER, COUNTER_ROT}, {UBR_CORNER, CLOCKWISE_ROT} },
	{ {DBL_CORNER, COUNTER_ROT}, {DBR_CORNER, CLOCKWISE_ROT}, {UBR_CORNER, COUNTER_ROT}, {UBL_CORNER, CLOCKWISE_ROT} },
	{ {DBR_CORNER, NO_ROT}, {DBL_CORNER, NO_ROT}, {DFL_CORNER, NO_ROT}, {DFR_CORNER, NO_ROT} }
};

/*
 * Lookup table mapping corner ids and slot rotations to the facelet at that rotation on the corner piece.
 */
static CubeFaceId cornerFacelets[NUM_CUBE_CORNERS][NUM_CUBE_ROTATIONS_PER_CORNER] = {
	{ U_FACE, F_FACE, R_FACE },
	{ U_FACE, L_FACE, F_FACE },
	{ U_FACE, B_FACE, L_FACE },
	{ U_FACE, R_FACE, B_FACE },
	{ D_FACE, R_FACE, F_FACE },
	{ D_FACE, F_FACE, L_FACE },
	{ D_FACE, L_FACE, B_FACE },
	{ D_FACE, B_FACE, R_FACE }
};

/*
 * Gets the corner slot on a face of a cube.
 * 
 * Corner slots are rotated so that applying their rotation to an unrotated corner in that slot will align the facelet belonging to the given face with the dominant face.
 */
struct Corner getCornerSlotOnFace(CubeFaceId faceId, CubeCornerOnFaceId cornerOnFaceId) {
	return cornerSlotsOnFace[faceId][cornerOnFaceId];
}

/*
 * Gets the facelet at the given rotation of a corner slot in a cube.
 * 
 * The facelet found is the one where applying the given rotation to the unrotated corner aligns the facelet with the dominant face.
 * For example, the UFR corner has an up facelet at no rotation, a front facelet at clockwise rotation, and a right facelet at a counter-clockwise rotation.
 */
CubeFaceId getFaceletOnCorner(CubeCornerId cornerId, CubeCornerRotation rotation) {
	return cornerFacelets[cornerId][rotation];
}

/*
 * Gets the corner on a face of a cube.
 * 
 * The corner's rotation is changed so that the given face is considered the dominant face, but the corner's dominant facelet remains the same.
 */
struct Corner getCornerOnFace(Cube cube, CubeFaceId faceId, CubeCornerOnFaceId faceCornerSlotId) {
	struct Corner cornerSlot = getCornerSlotOnFace(faceId, faceCornerSlotId);
	struct Corner corner = getCorner(cube, cornerSlot.id);
	struct Corner cornerOnFace = getRotatedCorner(corner, cornerSlot.rotation);
	return cornerOnFace;
}

/*
 * Gets the facelet at a corner position on a face.
 */
CubeFaceId getCornerFaceletOnFace(Cube cube, CubeFaceId faceId, CubeCornerOnFaceId faceCornerSlotId) {
	struct Corner cornerOnFace = getCornerOnFace(cube, faceId, faceCornerSlotId);
	CubeFaceId faceletId = getFaceletOnCorner(cornerOnFace.id, cornerOnFace.rotation);
	return faceletId;
}