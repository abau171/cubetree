#ifndef TURN_INTERNAL_H
#define TURN_INTERNAL_H

void quarterRevolveCorners(Cube cube, CubeFaceId faceId, bool clockwise);

void quarterRotateCorners(Cube cube, CubeFaceId faceId, bool clockwise);

void quarterRevolveEdges(Cube cube, CubeFaceId faceId, bool clockwise);

void quarterFlipEdges(Cube cube, CubeFaceId faceId, bool clockwise);

#endif