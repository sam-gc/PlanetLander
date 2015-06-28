#ifndef MESH_H
#define MESH_H

#include <SDL_opengl.h>

typedef struct {
    GLuint VAO;
    GLuint positionBuffer;
    GLuint colorBuffer;
    GLuint indexBuffer;

    int vCount;
    int iCount;
} Mesh;

Mesh mh_make(GLfloat *points, int count);

#endif
