#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "core/terrain.h"
#include "core/game.h"
#include "tools/glt_tools.h"

#define GRANULARITY 3000
#define SEVERITY 60
#define EXTRA 10

/*
void tr_gen(GLfloat *buf, int ct, int severity)
{
    if(ct < 3)
        return;

    int half = ct / 2;

    float avg = (buf[1] + buf[ct - 1]) / 2;
    if(severity > 0)
        avg += rand() % severity - severity / 2;
    
    buf[half + 1] = avg;

    tr_gen(buf, ct / 2, severity * 0.5);
    tr_gen(buf + ct / 2, ct / 2, severity * 0.5);
}*/

GLfloat tr_gen_up(GLfloat *buf, int ct, int severity, int where)
{
    buf++;
    GLfloat first = *buf++;

    int i;
    for(i = 0; i < ct; i++)
    {
        int amt = rand() % severity + 1;
        if(first + amt > 450 && i < GRANULARITY / EXTRA)
            amt = 0;

        buf[i * 2 + 1] = first + amt;
        first += amt;
    }

    return first;
}

void tr_gen_flat(GLfloat *buf, int ct)
{
    buf++;
    GLfloat first = *buf++;

    int i;
    for(i = 0; i < ct; i++)
        buf[i * 2 + 1] = first;
}

GLfloat tr_gen_down(GLfloat *buf, int ct, int severity)
{
    buf++;
    GLfloat first = *buf++;

    int i;
    for(i = 0; i < ct; i++)
    {
        int amt = rand() % severity + 1;
        if(first - amt < 0)
            amt = 0;
        
        buf[i * 2 + 1] = first - amt;
        first -= amt;
    }

    return first;
}

void tr_gen(GLfloat *buf, int ct, int severity)
{
    int total = 0;
    int i;
    for(i = 0; i < ct; i++)
    {
        int which = rand() % 3;
        int length = rand() % 7;

        length = length + total >= ct ? ct - total : length;

        if(which == 0)
            tr_gen_down(buf, length, severity);
        else if(which == 1)
            tr_gen_up(buf, length, severity, i);
        else
        {
            tr_gen_flat(buf, length);
        }

        buf += length * 2;
        total += length;
    }
}

double tr_altitude_at(Terrain *t, double x, double y)
{
    float amtpr = (glob_game.ppw * EXTRA) / (GRANULARITY / 2);
    int idx = (int)(x / amtpr) * 2;

    return y - t->data[idx + 1];
}

void tr_gen_matrix(Terrain *terrain)
{
    mat4x4 ident, temp, trans;
    mat4x4_identity(ident);

    /*
    float glx = 0;
    float gly = 2 / glob_game.pph;

    gly *= 10;

    glx--;
    gly--;

    float scale = 2 / glob_game.ppw;

    */


    //mat4x4_scale_aniso(temp, ident, scale, scale, scale);
    
    //mat4x4_translate(trans, glx, gly, 0);
    //mat4x4_mul(temp, trans, temp);
    mat4x4_transpose(terrain->mvMatrix, ident);
}

Terrain tr_make()
{
    GLfloat buf[GRANULARITY];

    
    buf[0] = 0;
    buf[1] = 10;

    //buf[GRANULARITY - 2] = glob_game.ppw;
    //buf[GRANULARITY - 1] = 10;
    double step = (glob_game.ppw * EXTRA) / (GRANULARITY / 2);

    int i;
    for(i = 0; i < GRANULARITY / 2; i++)
    {
        buf[i * 2] = i * step;
    }

    tr_gen(buf, GRANULARITY / 2, SEVERITY);

    Terrain terrain;
    terrain.mesh = mh_make(buf, NULL, GRANULARITY / 2);
    mh_set_u_color(&terrain.mesh, 1, 1, 1);

    tr_gen_matrix(&terrain);

    GLfloat *data = malloc(GRANULARITY * sizeof(GLfloat));
    memcpy(data, buf, GRANULARITY * sizeof(GLfloat));
    terrain.data = data;

    return terrain;
}

void tr_render_ex(Terrain *terrain, int idx)
{
    mat4x4 ident, mv;
    mat4x4_identity(ident);
    mat4x4_translate_in_place(ident, glob_game.ppw * EXTRA * idx, 0, 0);
    mat4x4_transpose(mv, ident);

    glUniformMatrix4fv(glob_locs.uMVMatrix, 1, GL_FALSE, (GLfloat *)mv);

    glBindVertexArray(terrain->mesh.VAO);
    mh_prepare_for_render(&terrain->mesh);
    glDrawArrays(GL_LINE_STRIP, 0, GRANULARITY / 2);
    glBindVertexArray(0);

    if(cam_point_is_visible(glob_game.camera, glob_game.ppw * idx - 1, glob_game.lander->y) && idx <= 0)
        tr_render_ex(terrain, idx - 1);
    if(cam_point_is_visible(glob_game.camera, glob_game.ppw * EXTRA * idx + glob_game.ppw * EXTRA + 1, glob_game.lander->y) && idx >= 0)
        tr_render_ex(terrain, idx + 1);
}

void tr_render(Terrain *terrain)
{
    tr_render_ex(terrain, 0);
}
