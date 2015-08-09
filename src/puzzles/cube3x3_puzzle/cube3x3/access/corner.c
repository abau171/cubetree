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
 * Returns a rotated version of the given corner.
 */
struct Corner getRotatedCorner(struct Corner corner, CubeCornerRotation dRotation) {
	corner.rotation = (corner.rotation + dRotation) % NUM_CUBE_ROTATIONS_PER_CORNER;
	return corner;
}

/*
 * Rotates a corner in a direction.
 */
void rotateCorner(Cube cube, CubeCornerId cornerSlotId, CubeCornerRotation dRotation) {
	struct Corner corner = getCorner(cube, cornerSlotId);
	struct Corner rotatedCorner = getRotatedCorner(corner, dRotation);
	setCorner(cube, cornerSlotId, rotatedCorner);
}