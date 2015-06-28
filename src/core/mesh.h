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

    GLfloat uColor[3];
} Mesh;

Mesh mh_make(GLfloat *points, GLfloat *colors, int ecount);
void mh_set_u_color(Mesh *mesh, GLfloat r, GLfloat g, GLfloat b);
void mh_prepare_for_render(Mesh *mesh);

#endif
