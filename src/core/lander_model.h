#ifndef LANDER_MODEL_H
#define LANDER_MODEL_H

#include "core/mesh.h"

typedef struct {
    Mesh mesh;
} Lander;

Lander lndr_new();
void lndr_render(Lander *lander);

#endif
