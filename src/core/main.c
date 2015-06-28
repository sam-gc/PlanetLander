#include <stdio.h>
#include "tools/glt_tools.h"
#include "core/lander_model.h"

// #define DEBUG_FRAMERATE

static Lander lander;

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    lndr_render(&lander);
    SDL_GL_SwapWindow(glob_info.window);
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

    lander = lndr_new();

    loop();

    glt_shutdown();

    return 0;
}

