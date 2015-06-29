#ifndef LANDER_MODEL_H
#define LANDER_MODEL_H

#include "tools/linmath.h"
#include "core/mesh.h"

typedef struct {
    Mesh mesh;
    mat4x4 mvMatrix;

    double dY;
    double dYY;
    double dX;

    double x;
    double y;
} Lander;

Lander lndr_new();
void lndr_step(Lander *lander, float dT);
void lndr_render(Lander *lander);

#endif
