#ifndef GLT_TOOLS_H
#define GLT_TOOLS_H

#include <SDL.h>
#include <SDL_opengl.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define ASPECT_RATIO (SCREEN_WIDTH / (float)SCREEN_HEIGHT)

typedef struct {
    GLuint program;
    GLuint vertexShader;
    GLuint fragmentShader;
    SDL_Window *window;
    SDL_GLContext context;
} ProgramInfo;

typedef struct {
    GLint aVertexPosition;
    GLint aColor;
    
    GLint uPerVertexColor;
    GLint uColor;
    GLint uMVMatrix;
} ProgramLocations;

extern ProgramInfo glob_info;
extern ProgramLocations glob_locs;

long glt_millis();
int glt_init();
void glt_shutdown();

#endif
