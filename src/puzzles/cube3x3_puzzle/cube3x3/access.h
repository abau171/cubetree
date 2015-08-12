#ifndef ACCESS_H
#define ACCESS_H

#include "cube.h"
#include "model.h"

// Corner Access

struct Corner getCorner(Cube cube, CubeCornerId cornerSlotId);

void setCorner(Cube cube, CubeCornerId cornerSlotId, struct Corner corner);

struct Corner getRotatedCorner(struct Corner corner, CubeCornerRotation dRotation);

void rotateCorner(Cube cube, CubeCornerId cornerSlotId, CubeCornerRotation dRotation);

// Corner Face Access

struct Corner getCornerSlotOnFace(CubeFaceId faceId, CubeCornerOnFaceId cornerOnFaceId);

CubeFaceId getFaceletOnCorner(CubeCornerId cornerId, CubeCornerRotation rotation);

struct Corner getCornerOnFace(Cube cube, CubeFaceId faceId, CubeCornerOnFaceId cornerOnFaceId);

CubeFaceId getCornerFaceletOnFace(Cube cube, CubeFaceId faceId, CubeCornerOnFaceId cornerOnFaceId);

// Edge Access

struct Edge getEdge(Cube cube, CubeEdgeId edgeSlotId);

void setEdge(Cube cube, CubeEdgeId edgeSlotId, struct Edge edge);

struct Edge getFlippedEdge(struct Edge edge, CubeEdgeFlip dFlip);

void flipEdge(Cube cube, CubeEdgeId edgeSlotId, CubeEdgeFlip dFlip);

// Edge Face Access

struct Edge getEdgeSlotOnFace(CubeFaceId faceId, CubeEdgeOnFaceId edgeOnFaceId);

CubeFaceId getFaceletOnEdge(CubeEdgeId edgeId, CubeEdgeFlip flip);

struct Edge getEdgeOnFace(Cube cube, CubeFaceId faceId, CubeEdgeOnFaceId edgeOnFaceId);

CubeFaceId getEdgeFaceletOnFace(Cube cube, CubeFaceId faceId, CubeEdgeOnFaceId edgeOnFaceId);

#endif