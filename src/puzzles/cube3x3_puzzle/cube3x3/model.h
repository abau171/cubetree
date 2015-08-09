#ifndef MODEL_H
#define MODEL_H

#include <stdint.h>

#include "cube.h"

#define NUM_CUBE_CORNERS_PER_FACE 4
#define NUM_CUBE_ROTATIONS_PER_CORNER 3

#define wrapEdgeFlip(flip) ((flip) % 2)

/*
 * Ids for each corner piece in a cube.
 */
typedef int8_t CubeCornerId;
enum CubeCornerId {
	UFR_CORNER,
	UFL_CORNER,
	UBL_CORNER,
	UBR_CORNER,
	DFR_CORNER,
	DFL_CORNER,
	DBL_CORNER,
	DBR_CORNER
};

/*
 * Ids for each edge piece in a cube.
 */
typedef int8_t CubeEdgeId;
enum CubeEdgeId {
	UF_EDGE,
	UL_EDGE,
	UB_EDGE,
	UR_EDGE,
	FR_EDGE,
	FL_EDGE,
	BL_EDGE,
	BR_EDGE,
	DF_EDGE,
	DL_EDGE,
	DB_EDGE,
	DR_EDGE
};

/*
 * Ids for the current rotation state of a corner piece in a cube.
 *
 * An unrotated corner piece is determined to be a corner piece whose dominant
 * face (up or down face, depending on which face the corner has) is facing
 * either up or down (depending on the corner slot it currently fills).  The
 * clockwise and counter-clockwise states are determined relative to the
 * unrotated state.
 *
 * This type can also be used as a change in rotation (for example, supplying
 * CLOCKWISE_ROT as a change in rotation would rotate a corner clockwise).
 */
typedef int8_t CubeCornerRotation;
enum CubeCornerRotation { NO_ROT, CLOCKWISE_ROT, COUNTER_ROT };

/*
 * Ids for the current flip state of an edge piece in a cube.
 *
 * The flip state of an edge piece is determined by the piece's dominant face
 * (up or down face if the piece has one, and front or back if not).  If the
 * dominant face is aligned with the dominant face of the edge slot it
 * occupies, it is considered unflipped.  Otherwise it is flipped.
 *
 * This type can also be used as a toggle for a flip state.  Applying FLIP to
 * a flip state reverses it, and applying NO_FLIP has no change.
 */
typedef int8_t CubeEdgeFlip;
enum CubeEdgeFlip { NO_FLIP, FLIP };

/*
 * Ids for the corners located on an unspecified face.
 *
 * The orientation of each face must be considered when identifying the
 * corners belonging to it in any order.  The up face is oriented so the front
 * face is pointing down and the back face is pointing up.  The down face is
 * oriented so the front face is pointing up and the back face is pointing
 * down.  The remaining 4 faces are oriented so the up face points up and the
 * down face points down.
 */
typedef int8_t CubeFaceCornerSlotId;
enum CubeFaceCornerSlotId {
	DR_FCS,
	DL_FCS,
	UL_FCS,
	UR_FCS
};

/*
 * Structure for a corner.  Each one has an Id and a current rotation.
 */
struct Corner {
	CubeCornerId id;
	CubeCornerRotation rotation;
};

/*
 * Structure for an edge.  Each one has an Id and a current flip state.
 */
struct Edge {
	CubeEdgeId id;
	CubeEdgeFlip flip;
};

/*
 * Structure for a cube.  Each cube has 8 corners and 12 edges.
 */
struct Cube {
	struct Corner corners[NUM_CUBE_CORNERS];
	struct Edge edges[NUM_CUBE_EDGES];
};

#endif
