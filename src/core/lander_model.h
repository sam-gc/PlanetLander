#ifndef LANDER_MODEL_H
#define LANDER_MODEL_H

#define LANDER_FOOT_LEFT_INDEX 34
#define LANDER_FOOT_RIGHT_INDEX 36

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
    Mesh boundingBoxMesh;

    int rendersBoundingBox;

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
void lndr_get_current_bounding_box(Lander *lander, GLfloat *buf);
void lndr_get_current_points(Lander *lander, GLfloat *buf);

#endif
