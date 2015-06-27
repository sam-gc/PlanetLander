#include <stdio.h>
#include <GL/glew.h>
#include "glt_tools.h"
#include "tools/linmath.h"

const char *vertex_shader_code = 
#include "shaders/vertex__gen.h"
;

const char *fragment_shader_code =
#include "shaders/fragment__gen.h"
;

/*typedef struct {
    GLuint program;
    GLuint vertexShader;
    GLuint fragmentShader;
    SDL_Window *window;
    SDL_GLContext context;
} ProgramInfo;

typedef struct {
    GLint aVertexPosition;
    GLint uMVMatrix;
} ProgramLocations;*/

ProgramInfo glob_info = {
    0,
    0,
    0,
    NULL,
    NULL
};

ProgramLocations glob_locs = {
    -1,
    -1
};

int glt_init_gl()
{
    return 1;
}

int glt_init()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize SDL. Error: %s\n", SDL_GetError());
        return 0;
    }

    glob_info.window = SDL_CreateWindow("Planet Lander", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if(!glob_info.window)
    {
        printf("Failed to load window. Error: %s\n", SDL_GetError());
        return 0;
    }

    glob_info.context = SDL_GL_CreateContext(glob_info.window);
    if(!glob_info.context)
    {
        printf("Could not create OpenGL context. Error %s\n", SDL_GetError());
        return 0;
    }

    SDL_GL_MakeCurrent(glob_info.window, glob_info.context);
    printf("%s\n", glGetString(GL_VERSION));

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if(glewError != GLEW_OK)
    {
        printf("GLEW initialization failed.\n");
        return 0;
    }

    if(SDL_GL_SetSwapInterval(1) < 0)
    {
        printf("Failed to use VSync.\n");
        return 0;
    }

    if(!glt_init_gl())
    {
        printf("Unable to initialize OpenGL.\n");
        return 0;
    }

    return 1;
}
