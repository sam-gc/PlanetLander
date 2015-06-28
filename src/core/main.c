#include <stdio.h>
#include <GL/glew.h>
#include "tools/glt_tools.h"
#include "tools/linmath.h"
#include "core/lander_model.h"

// #define DEBUG_FRAMERATE

static Lander lander;
static mat4x4 perspectiveMatrix;

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUniformMatrix4fv(glob_locs.uPMatrix, 1, GL_FALSE, (GLfloat *)perspectiveMatrix);

    lndr_render(&lander);
    SDL_GL_SwapWindow(glob_info.window);
}

void resize(SDL_Event *e)
{
    if(e->window.event != SDL_WINDOWEVENT_RESIZED)
        return;

    int width = e->window.data1;
    int height = e->window.data2;
    float aspect = (float)width / height;

    glob_info.winfo.width = width;
    glob_info.winfo.height = height;
    glob_info.winfo.aspectRatio = aspect;

    glViewport(0, 0, width, height);
    glt_build_perspective_matrix(&perspectiveMatrix);
}

void loop()
{
    SDL_Event e;
    int quit = 0;

#ifdef DEBUG_FRAMERATE
    long then = glt_millis();
    int frames = 0;
#endif

    while(!quit)
    {
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_QUIT:
                    quit = 1;
                    break;
                    
                case SDL_WINDOWEVENT:
                    resize(&e);
                    break;
            }
        }

        render();

#ifdef DEBUG_FRAMERATE
        if(glt_millis() - then >= 1000)
        {
            then = glt_millis();
            printf("Frame rate: %d\n", frames);
            frames = 0;
        }

        frames++;
#endif
    }
}

int main(int argc, char *argv[])
{
    if(!glt_init())
    {
        printf("Failure!!!\n");
        return -1;
    }

    glt_build_perspective_matrix(&perspectiveMatrix);

    lander = lndr_new();

    loop();

    glt_shutdown();

    return 0;
}

