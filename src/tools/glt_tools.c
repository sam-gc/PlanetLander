#include <stdio.h>
#include <GL/glew.h>
#include <sys/timeb.h>
#include "glt_tools.h"
#include "tools/linmath.h"

const char *vertex_shader_code[] = {
#include "shaders/vertex__gen.h"
};

const char *fragment_shader_code[] = {
#include "shaders/fragment__gen.h"
};

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

long glt_millis()
{
    struct timeb tb;
    ftime(&tb);
    return (long)(tb.time * 1000 + tb.millitm);
}

int glt_debug_shader(GLuint shader, char *which)
{
    GLint success = 0;
    GLint logSize = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(success != GL_FALSE)
        return 1;

    printf("Shader compilation (%s) failed:\n", which);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    char buf[logSize];

    glGetShaderInfoLog(shader, logSize, NULL, buf);
    printf("%s\n", buf);

    return 0;
}

int glt_init_gl()
{
    glob_info.program = glCreateProgram();
    glob_info.vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(glob_info.vertexShader, 1, vertex_shader_code, NULL);
    glCompileShader(glob_info.vertexShader);

    if(!glt_debug_shader(glob_info.vertexShader, "Vertex"))
        return 0;

    glAttachShader(glob_info.program, glob_info.vertexShader);

    glob_info.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(glob_info.fragmentShader, 1, fragment_shader_code, NULL);
    glCompileShader(glob_info.fragmentShader);

    if(!glt_debug_shader(glob_info.fragmentShader, "Fragment"))
        return 0;

    glAttachShader(glob_info.program, glob_info.fragmentShader);

    glLinkProgram(glob_info.program);

    glob_locs.aVertexPosition = glGetAttribLocation(glob_info.program, "aPosition");
    glob_locs.uMVMatrix = glGetUniformLocation(glob_info.program, "uMV");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return 1;
}

int glt_init()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize SDL. Error: %s\n", SDL_GetError());
        return 0;
    }

#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
#endif

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

void glt_shutdown()
{
    glDeleteProgram(glob_info.program);
    SDL_DestroyWindow(glob_info.window);

    glob_info.window = NULL;

    SDL_Quit();
}

