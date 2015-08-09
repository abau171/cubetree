#include "../cube.h"
#include "../model.h"
#include "../access.h"

/*
 * Gets the corner at a corner slot in a cube.
 */
struct Corner getCorner(Cube cube, CubeCornerId cornerSlotId) {
	return cube->corners[cornerSlotId];
}

/*
 * Sets the corner at a corner slot in a cube.
 */
void setCorner(Cube cube, CubeCornerId cornerSlotId, struct Corner corner) {
	cube->corners[cornerSlotId] = corner;
}

/*
 * Rotates a corner in a direction.
 */
void rotateCorner(Cube cube, CubeCornerId cornerSlotId, CubeCornerRotation dRotation) {
	struct Corner corner = getCorner(cube, cornerSlotId);
	corner.rotation = (corner.rotation + dRotation) % NUM_CUBE_ROTATIONS_PER_CORNER;
	setCorner(cube, cornerSlotId, corner);
}

/*
 * Lookup table mapping face and face corner id pairs to corner slot ids.
 */
static CubeCornerId cornerSlotIdsByFace[NUM_CUBE_FACES][NUM_CUBE_CORNERS_PER_FACE] = {
	{UFR_CORNER, UFL_CORNER, UBL_CORNER, UBR_CORNER},
	{DFL_CORNER, DBL_CORNER, UBL_CORNER, UFL_CORNER},
	{DFR_CORNER, DFL_CORNER, UFL_CORNER, UFR_CORNER},
	{DBR_CORNER, DFR_CORNER, UFR_CORNER, UBR_CORNER},
	{DBL_CORNER, DBR_CORNER, UBR_CORNER, UBL_CORNER},
	{DBR_CORNER, DBL_CORNER, DFL_CORNER, DFR_CORNER}
};

/*
 * Gets the corner slot id in a cube which is at a location relative to a face of the cube.
 */
CubeCornerId getCornerSlotIdByFace(CubeFaceId faceId, CubeFaceCornerSlotId faceCornerSlotId) {
	return cornerSlotIdsByFace[faceId][faceCornerSlotId];
}

/*
 * Gets the corner in a cube at a location relative to a face of the cube.
 */
struct Corner getCornerByFace(Cube cube, CubeFaceId faceId, CubeFaceCornerSlotId faceCornerSlotId) {
	CubeCornerId cornerSlotId = getCornerSlotIdByFace(faceId, faceCornerSlotId);
	return getCorner(cube, cornerSlotId);
}

/*
 * Lookup table mapping face and face corner id pairs to corner slot rotations.
 */
static CubeCornerRotation cornerSlotRotationsByFace[NUM_CUBE_FACES][NUM_CUBE_CORNERS_PER_FACE] = {
	{NO_ROT, NO_ROT, NO_ROT, NO_ROT},
	{CLOCKWISE_ROT, COUNTER_ROT, COUNTER_ROT, CLOCKWISE_ROT},
	{CLOCKWISE_ROT, COUNTER_ROT, COUNTER_ROT, CLOCKWISE_ROT},
	{CLOCKWISE_ROT, COUNTER_ROT, COUNTER_ROT, CLOCKWISE_ROT},
	{CLOCKWISE_ROT, COUNTER_ROT, COUNTER_ROT, CLOCKWISE_ROT},
	{NO_ROT, NO_ROT, NO_ROT, NO_ROT}
};

/*
 * Gets the rotation of the face of a corner slot in a cube.  This rotation is
 * the same as the rotation a corner would need to be in for its dominant face
 * to be on the specified face.
 */
CubeCornerRotation getCornerSlotFaceRotation(CubeFaceId faceId, CubeFaceCornerSlotId faceCornerSlotId) {
	return cornerSlotRotationsByFace[faceId][faceCornerSlotId];
}

/*
 * Gets the rotation of the face of a corner in a cube.  The face of the slot
 * at this rotation is the face of the cube that the considered corner's face
 * belongs to.
 */
CubeCornerRotation getCornerFaceRotation(Cube cube, CubeFaceId faceId, CubeFaceCornerSlotId faceCornerSlotId) {
	CubeCornerRotation cornerSlotFaceRotation = getCornerSlotFaceRotation(faceId, faceCornerSlotId);
	CubeCornerRotation cornerRotation = getCornerByFace(cube, faceId, faceCornerSlotId).rotation;
	return (NUM_CUBE_ROTATIONS_PER_CORNER + cornerSlotFaceRotation - cornerRotation) % NUM_CUBE_ROTATIONS_PER_CORNER;
}

/*
 * Lookup table mapping corner id and corner rotation to the face that the
 * corner face belongs to.
 */
static CubeFaceId faceByCornerRotation[NUM_CUBE_CORNERS][NUM_CUBE_ROTATIONS_PER_CORNER] = {
	{U_FACE, R_FACE, F_FACE},
	{U_FACE, F_FACE, L_FACE},
	{U_FACE, L_FACE, B_FACE},
	{U_FACE, B_FACE, R_FACE},
	{D_FACE, F_FACE, R_FACE},
	{D_FACE, L_FACE, F_FACE},
	{D_FACE, B_FACE, L_FACE},
	{D_FACE, R_FACE, B_FACE}
};

/*
 * Gets the face that the corner face at a given rotation belongs to.
 */
CubeFaceId getFaceByCornerRotation(CubeCornerId cornerId, CubeCornerRotation rotation) {
	return faceByCornerRotation[cornerId][rotation];
}