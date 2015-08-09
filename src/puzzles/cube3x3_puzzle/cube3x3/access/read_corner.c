#include "../cube.h"
#include "../model.h"

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
	corner.rotation = (corner.rotation + dRotation) % 3;
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

