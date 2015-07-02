#ifndef TERRAIN_H
#define TERRAIN_H

#include "tools/linmath.h"
#include "core/mesh.h"

typedef struct {
    Mesh mesh;
    mat4x4 mvMatrix;
    GLfloat *data;
} Terrain;

Terrain tr_make();
void tr_render(Terrain *terrain);
int tr_test_bounding_box(Terrain *terrain, GLfloat *bb);
int tr_test_collisions(Terrain *t, GLfloat *pts, int count);
double tr_altitude_at(Terrain *t, double x, double y);
double tr_accurate_altitude_at(Terrain *t, double x, double y);

#endif
