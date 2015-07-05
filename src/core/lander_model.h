#ifndef LANDER_MODEL_H
#define LANDER_MODEL_H

#define LANDER_FOOT_LEFT_INDEX 68
#define LANDER_FOOT_RIGHT_INDEX 72

#include "tools/linmath.h"
#include "core/mesh.h"

typedef enum {
    JS_OFF = 0,
    JS_INCREASING,
    JS_ON,
    JS_DECREASING
} JetState;

typedef enum {
    LR_PERFECT,
    LR_GOOD,
    LR_OKAY,
    LR_HARD,
    LR_CRASH
} LandingResult;

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
LandingResult lndr_interpret_landing_result(Lander *lander, double lalt, double ralt);
void lndr_print_diagnostics(Lander *lander);

#endif
