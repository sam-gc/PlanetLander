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
double tr_altitude_at(Terrain *t, double x, double y);

#endif
