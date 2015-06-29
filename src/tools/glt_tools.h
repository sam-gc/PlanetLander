#ifndef GLT_TOOLS_H
#define GLT_TOOLS_H

#include "tools/platform_headers.h"
#include "tools/linmath.h"

typedef struct {
    int width;
    int height;
    float aspectRatio;
} WindowInfo;

typedef struct {
    GLuint program;
    GLuint vertexShader;
    GLuint fragmentShader;
    SDL_Window *window;
    SDL_GLContext context;

    WindowInfo winfo;
} ProgramInfo;

typedef struct {
    GLint aVertexPosition;
    GLint aColor;
    
    GLint uPerVertexColor;
    GLint uColor;
    GLint uMVMatrix;
    GLint uPMatrix;
} ProgramLocations;

extern ProgramInfo glob_info;
extern ProgramLocations glob_locs;

long glt_millis();
int glt_init();
void glt_shutdown();
void glt_build_perspective_matrix(mat4x4 *P);

#endif
