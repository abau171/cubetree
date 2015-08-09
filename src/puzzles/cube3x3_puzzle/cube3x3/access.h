#ifndef ACCESS_H
#define ACCESS_H

#include "cube.h"
#include "model.h"

struct Corner getCorner(Cube cube, CubeCornerId cornerSlotId);

void setCorner(Cube cube, CubeCornerId cornerSlotId, struct Corner corner);

struct Corner getRotatedCorner(struct Corner corner, CubeCornerRotation dRotation);

void rotateCorner(Cube cube, CubeCornerId cornerSlotId, CubeCornerRotation dRotation);

CubeCornerId getCornerSlotIdOnFace(CubeFaceId faceId, CubeCornerOnFaceId faceCornerSlotId);

struct Corner getCornerOnFace(Cube cube, CubeFaceId faceId, CubeCornerOnFaceId faceCornerSlotId);

CubeCornerRotation getCornerSlotFaceRotation(CubeFaceId faceId, CubeCornerOnFaceId faceCornerSlotId);

CubeCornerRotation getCornerFaceletRotation(Cube cube, CubeFaceId faceId, CubeCornerOnFaceId faceCornerSlotId);

CubeFaceId getFaceletByCornerRotation(CubeCornerId cornerId, CubeCornerRotation rotation);

CubeFaceId getCornerFaceletOnFace(Cube cube, CubeFaceId faceId, CubeCornerOnFaceId faceCornerSlotId);

#endif