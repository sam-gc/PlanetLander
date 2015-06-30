#include <stdio.h>
#include <GL/glew.h>
#include "tools/glt_tools.h"
#include "tools/linmath.h"
#include "core/lander_model.h"
#include "core/game.h"
#include "core/mesh.h"
#include "core/terrain.h"
#include "core/camera.h"

static Lander lander;
static Terrain terrain;
static Camera camera;
static mat4x4 perspectiveMatrix;

float Lx = -1;
float Ly = -1;
float Lw = -1;
float Lh = -1;

GameInfo glob_game = {0, 1.0, 1000, 1000, 0};
//#define DEBUG_FRAMERATE

double max(double a, double b)
{
    return a > b ? a : b;
}

void gm_render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // The below code will cause the camera to follow the lander and scale it up without
    // affecting game speed.
    
    /*
    if(tr_altitude_at(&terrain, lander.x, lander.y) < 150)
    {
        mat4x4 p, temp, ident;
        mat4x4_identity(ident);
        mat4x4_scale_aniso(temp, ident, 2, 2, 2);

        float w = glob_game.ppw;
        float h = glob_game.pph;

        if(Lx < 0)
        {
            Lx = lander.x - w / 2;
            Ly = lander.y - h / 2;
            Lw = w;
            Lh = h;
        }
        else if(lander.x - 10 < Lx)
        {
            mat4x4_translate_in_place(temp, (-2 / glob_game.ppw) * (lander.x - 10) + 1, 0, 0);
            Lx = lander.x - 10;
        }
        else if(lander.x + 10 > Lx + Lw)
        {
            mat4x4_translate_in_place(temp, (-2 / glob_game.ppw) * (lander.x + ))
        }
        mat4x4_translate_in_place(temp, (-2 / glob_game.ppw) * lander.x + 1, (-2 / glob_game.pph) * lander.y + 1, 0);
        mat4x4_transpose(p, temp);
        mat4x4_mul(temp, p, perspectiveMatrix);

        glUniformMatrix4fv(glob_locs.uPMatrix, 1, GL_FALSE, (GLfloat *)temp);
    }
    else*/
    cam_prepare_matrix(&camera);

    glUniformMatrix4fv(glob_locs.uPMatrix, 1, GL_FALSE, (GLfloat *)camera.matrix);

    lndr_render(&lander);
    tr_render(&terrain);

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

    camera = cam_make();

    glViewport(0, 0, width, height);
    glt_build_perspective_matrix(&perspectiveMatrix);
}

void gm_handle_key_event(SDL_Event *e)
{
    SDL_KeyboardEvent evt = e->key;
    if(evt.type != SDL_KEYDOWN && evt.type != SDL_KEYUP)
        return;

    unsigned mappedKey = 0;
    switch(evt.keysym.sym)
    {
        case SDLK_UP:
            mappedKey = GMK_UP;
            break;
        case SDLK_DOWN:
            mappedKey = GMK_DOWN;
            break;
        case SDLK_LEFT:
            mappedKey = GMK_LEFT;
            break;
        case SDLK_RIGHT:
            mappedKey = GMK_RIGHT;
            break;
        default:
            return;
    }

    if(evt.type == SDL_KEYDOWN)
        glob_game.keysDown |= mappedKey;
    else
        glob_game.keysDown ^= mappedKey;
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

                case SDL_KEYUP:
                case SDL_KEYDOWN:
                    gm_handle_key_event(&e);
                    break;
            }
        }

        gm_render();

        long now = SDL_GetTicks();
        float dT = (now - ticks) / 1000.0;
        ticks = now;

        lndr_step(&lander, dT);

        float alt = tr_altitude_at(&terrain, lander.x, lander.y);
        if(alt < 150 && camera.scale == 1.0)
        {
            cam_set_zoom(&camera, 2.0);
            cam_center_on(&camera, lander.x, lander.y);
        }
        else if(alt > 150 && camera.scale != 1.0)
        {
            cam_set_zoom(&camera, 1.0);
            camera.x = camera.y = 0;
        }
        else if(camera.scale > 1.0)
        {
            double x, y;
            x = y = 0;
            if(camera.x + camera.w / 4 > lander.x)
                x = -lander.dX * dT;
            /*
            else if(camera.x - camera.w / 3 > lander.x)
                x = 1;
                */

            cam_pan(&camera, x, -lander.dY * dT);
        }

#ifdef DEBUG_FRAMERATE
        if(glt_millis() - then >= 250)
        {
        printf("%lf\n", tr_altitude_at(&terrain, lander.x, lander.y));
            then = glt_millis();
            //printf("Frame rate: %d\n", frames);
            frames = 0;
        }

        frames++;
#endif
    }
}

void gm_start()
{
    glt_build_perspective_matrix(&perspectiveMatrix);

    camera = cam_make();
    lander = lndr_new();
    terrain = tr_make();
    gm_loop();
}

