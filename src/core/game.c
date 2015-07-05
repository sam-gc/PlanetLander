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

GameInfo glob_game = {0, 1.0, 0, 1000, 1000, 0};
//#define DEBUG_FRAMERATE

double max(double a, double b)
{
    return a > b ? a : b;
}

double min(double a, double b)
{
    return a < b ? a : b;
}

void gm_render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // The below code will cause the camera to follow the lander and scale it up without
    // affecting game speed.
    
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

    SDL_Delay(50);

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

    if(glob_game.landerIsLanded)
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

void gm_position_camera(double dT)
{

    float alt = tr_altitude_at(&terrain, lander.x, lander.y);
    if(alt < 150 && camera.scale <= 1.0)
    {
        cam_set_zoom(&camera, 2.0);
        cam_center_on(&camera, lander.x, lander.y - camera.h * 0.4);
    }
    else if(alt > 150 && camera.scale > 1.0)
    {
        cam_set_zoom(&camera, 1.0);
        camera.x = camera.y = 0;
    }
    else if(camera.scale > 1.0)
    {
        double x, y;
        x = y = 0;
        if(camera.x + camera.w / 8 > lander.x && lander.dX < 0)
            x = lander.dX * dT;
        else if(camera.x + camera.w / 4 > lander.x && lander.dX < 0)
            x = max(lander.dX * dT, -0.4);
        else if(camera.x + camera.w * (7./8) < lander.x && lander.dX > 0)
            x = lander.dX * dT;
        else if(camera.x + camera.w * 0.75 < lander.x && lander.dX > 0)
            x = min(lander.dX * dT, 0.4);

        if(camera.y + camera.h / 8 > lander.y && lander.dY < 0)
            y = lander.dY * dT;
        else if(camera.y + camera.h / 4 > lander.y && lander.dY < 0)
            y = max(lander.dY * dT, -0.4);
        else if(camera.y + camera.h * (7./8) < lander.y && lander.dY > 0)
            y = lander.dY * dT;
        else if(camera.y + camera.h * 0.75 < lander.y && lander.dY > 0)
            y = min(lander.dY * dT, 0.4);

        cam_pan(&camera, x, y);
    }
    else if(alt > 150 && lander.y + 30 > camera.max_h)
    {
        double dif = lander.y - (camera.h - 30);
        double scale = glob_game.pph / (camera.h + dif);
        cam_set_zoom(&camera, scale);
        camera.x = camera.y = 0;
    }

    if(alt > 150)
    {
        double x, y;
        x = y = 0;
        if(camera.x + camera.w / 8 > lander.x && lander.dX < 0)
            x = lander.dX * dT;
        else if(camera.x + camera.w / 4 > lander.x && lander.dX < 0)
            x = max(lander.dX * dT, -0.4);
        else if(camera.x + camera.w * (7./8) < lander.x && lander.dX > 0)
            x = lander.dX * dT;
        else if(camera.x + camera.w * 0.75 < lander.x && lander.dX > 0)
            x = min(lander.dX * dT, 0.4);
        cam_pan(&camera, x, y);
    }
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

        gm_position_camera(dT);

        GLfloat points[112];
        lndr_get_current_points(&lander, points);
        if(!tr_test_collisions(&terrain, points, 56) && !glob_game.landerIsLanded)
        {
            double lalt = tr_accurate_altitude_at(&terrain, points[LANDER_FOOT_LEFT_INDEX], points[LANDER_FOOT_LEFT_INDEX + 1]);
            double ralt = tr_accurate_altitude_at(&terrain, points[LANDER_FOOT_RIGHT_INDEX], points[LANDER_FOOT_RIGHT_INDEX + 1]);

            glob_game.landerIsLanded = 1;
            printf("Foot 1: %lf\nFoot 2: %lf\n", lalt, ralt);
                    
            glob_game.keysDown = 0;

            lndr_print_diagnostics(&lander);
            LandingResult lr = lndr_interpret_landing_result(&lander, lalt, ralt);
            char *text;
            switch(lr)
            {
                case LR_CRASH:
                    text = "Crash!";
                    break;
                case LR_HARD:
                    text = "Hard";
                    break;
                case LR_OKAY:
                    text = "Okay";
                    break;
                case LR_GOOD:
                    text = "Good";
                    break;
                case LR_PERFECT:
                    text = "Perfect!";
                    break;
            }
            printf("Landing: %s\n", text);
            
            lander.dX = lander.dY = lander.dYY = 0;
        }

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

    camera = cam_make();

    glob_game.camera = &camera;

    lander = lndr_new();

    glob_game.lander = &lander;

    terrain = tr_make();
    gm_loop();
}

