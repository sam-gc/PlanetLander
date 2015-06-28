#include "GL/glew.h"
#include "core/mesh.h"
#include "tools/glt_tools.h"

Mesh mh_make(GLfloat *points, GLfloat *colors, int ecount)
{
    Mesh mesh;
    GLuint VAO;
    GLuint positionBuffer, colorBuffer;

    colorBuffer = 0;

    mesh.vCount = ecount;

    // Generate vertex array object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate buffers
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, ecount * 2 * sizeof(GLfloat), points, GL_STATIC_DRAW);

    if(colors)
    {
        glGenBuffers(1, &colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, ecount * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
    }

    mesh.VAO = VAO;
    mesh.positionBuffer = positionBuffer;
    mesh.colorBuffer = colorBuffer;


    if(colors)
    {
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glEnableVertexAttribArray(glob_locs.aColor);
        glVertexAttribPointer(glob_locs.aColor, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    }
    else
    {
        mesh.uColor[0] = mesh.uColor[2] = 1;
        mesh.uColor[1] = 0;
    }

    // Has '2' for second parameter 'size' since we are drawing lines.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glEnableVertexAttribArray(glob_locs.aVertexPosition);
    glVertexAttribPointer(glob_locs.aVertexPosition, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

    glBindVertexArray(0);

    return mesh;
}

void mh_set_u_color(Mesh *mesh, GLfloat r, GLfloat g, GLfloat b)
{
    mesh->uColor[0] = r;
    mesh->uColor[1] = g;
    mesh->uColor[2] = b;
}

void mh_prepare_for_render(Mesh *mesh)
{
    if(!mesh->colorBuffer)
    {
        glUniform3fv(glob_locs.uColor, 1, mesh->uColor);
        glUniform1i(glob_locs.uPerVertexColor, 0);
        return;
    }
    
    glUniform1i(glob_locs.uPerVertexColor, 1);
}

