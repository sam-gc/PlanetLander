#include <GL/glew.h>
#include <stdio.h>
#include "tools/glt_tools.h"
#include "tools/linmath.h"
#include "core/lander_model.h"
#include "core/game.h"

static Lander lander;
static mat4x4 perspectiveMatrix;

GameInfo glob_game = {0, 1.0, 1000, 1000};

void gm_render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUniformMatrix4fv(glob_locs.uPMatrix, 1, GL_FALSE, (GLfloat *)perspectiveMatrix);

    lndr_render(&lander);
    SDL_GL_SwapWindow(glob_info.window);
}

void gm_resize(SDL_Event *e)
{
    if(e->window.event != SDL_WINDOWEVENT_RESIZED)
        return;

    int width = e->window.data1;
    int height = e->window.data2;
    float aspect = (float)width / height;

    glob_info.winfo.width = width;
    glob_info.winfo.height = height;
    glob_info.winfo.aspectRatio = aspect;

    glob_game.pph = glob_game.ppw / aspect;

    glViewport(0, 0, width, height);
    glt_build_perspective_matrix(&perspectiveMatrix);
}

void gm_loop()
{
    SDL_Event e;
    int quit = 0;

#ifdef DEBUG_FRAMERATE
    long then = glt_millis();
    int frames = 0;
#endif

    long ticks = SDL_GetTicks();
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
                    gm_resize(&e);
                    break;
            }
        }

        gm_render();

        long now = SDL_GetTicks();
        float dT = (now - ticks) / 1000.0;
        ticks = now;

        lndr_step(&lander, dT);

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

void gm_start()
{
    glt_build_perspective_matrix(&perspectiveMatrix);

    lander = lndr_new();
    gm_loop();
}

