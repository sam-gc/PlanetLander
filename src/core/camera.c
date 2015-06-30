#include "core/camera.h"
#include "core/game.h"
#include <stdio.h>

Camera cam_make()
{
    Camera camera;
    camera.w = glob_game.ppw;
    camera.h = glob_game.pph;

    camera.max_w = camera.w;
    camera.max_h = camera.h;

    camera.x = 0;
    camera.y = 0;
    camera.scale = 1.0;

    return camera;
}

void cam_pan(Camera *cam, double x, double y)
{
    cam->x += x;
    cam->y += y;
}

void cam_center_on(Camera *cam, double x, double y)
{
    cam->x = x - cam->w / 2;
    cam->y = y - cam->h / 2;
}

void cam_set_zoom(Camera *cam, double zoom)
{
    cam->scale = zoom;
    cam->w = cam->max_w / zoom;
    cam->h = cam->max_h / zoom;
}

void cam_prepare_matrix(Camera *cam)
{
    mat4x4 ident, temp, trans;
    mat4x4_identity(ident);

    float glx = 2 / glob_game.ppw;
    float gly = 2 / glob_game.pph;

    /*
    glx *= (cam->x + cam->w / 2);
    gly *= (cam->y + cam->h / 2);
    glx--;
    gly--;

    */
    mat4x4_scale_aniso(temp, ident, cam->scale * glx, cam->scale * gly, 1);
    mat4x4_translate(trans, -(cam->x + cam->w / 2), -(cam->y + cam->h / 2), 0);
    mat4x4_mul(ident, temp, trans);

    /*
    mat4x4_identity(temp);
    mat4x4_scale_aniso(trans, temp, 1, -1, 1);
    mat4x4_mul(temp, trans, ident);
    */
    mat4x4_transpose(cam->matrix, ident);
}

