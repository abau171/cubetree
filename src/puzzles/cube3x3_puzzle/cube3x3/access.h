#ifndef ACCESS_H
#define ACCESS_H

#include "cube.h"
#include "model.h"

struct Corner getCorner(Cube cube, CubeCornerId cornerSlotId);

void setCorner(Cube cube, CubeCornerId cornerSlotId, struct Corner corner);

void rotateCorner(Cube cube, CubeCornerId cornerSlotId, CubeCornerRotation dRotation);

CubeCornerId getCornerSlotIdByFace(CubeFaceId faceId, CubeFaceCornerSlotId faceCornerSlotId);

struct Corner getCornerByFace(Cube cube, CubeFaceId faceId, CubeFaceCornerSlotId faceCornerSlotId);

CubeCornerRotation getCornerSlotFaceRotation(CubeFaceId faceId, CubeFaceCornerSlotId faceCornerSlotId);

CubeCornerRotation getCornerFaceRotation(Cube cube, CubeFaceId faceId, CubeFaceCornerSlotId faceCornerSlotId);

CubeFaceId getFaceByCornerRotation(CubeCornerId cornerId, CubeCornerRotation rotation);

#endif