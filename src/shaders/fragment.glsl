#version 140

out vec4 color;
in vec3 iColor;

uniform int uPerVertexColor;
uniform vec3 uColor;

void main() {
    if(uPerVertexColor == 1)
        color = vec4(iColor.rgb, 1.0);
    else
        color = vec4(uColor.rgb, 1.0);

    // color = vec4(1.0, 1.0, 1.0, 1.0);
}
