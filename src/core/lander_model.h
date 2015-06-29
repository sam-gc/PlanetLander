#ifndef LANDER_MODEL_H
#define LANDER_MODEL_H

#include "tools/linmath.h"
#include "core/mesh.h"

typedef enum {
    JS_OFF = 0,
    JS_INCREASING,
    JS_ON,
    JS_DECREASING
} JetState;

typedef struct {
    Mesh mesh;
    mat4x4 mvMatrix;

    double dY;
    double dYY;
    double dX;
    double dR;

    double rotation;

    double x;
    double y;

    Mesh jetMesh;
    mat4x4 jetMatrix;

    JetState jetState;
    JetState prevJetState;
    long jetFrames;

} Lander;

Lander lndr_new();
void lndr_step(Lander *lander, float dT);
void lndr_render(Lander *lander);

#endif
