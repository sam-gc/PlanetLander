#include "GL/glew.h"
#include "core/mesh.h"
#include "tools/glt_tools.h"

Mesh mh_make(GLfloat *points, int count)
{
    Mesh mesh;
    GLuint VAO;
    GLuint positionBuffer;

    mesh.vCount = count;

    // Generate vertex array object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate buffers
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), points, GL_STATIC_DRAW);

    mesh.VAO = VAO;
    mesh.positionBuffer = positionBuffer;

    glEnableVertexAttribArray(glob_locs.aVertexPosition);

    // Has '2' for second parameter 'size' since we are drawing lines.
    glVertexAttribPointer(glob_locs.aVertexPosition, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

    glBindVertexArray(0);

    return mesh;
}
