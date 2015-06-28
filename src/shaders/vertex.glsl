#version 140

uniform mat4 uMV;
uniform mat4 uP;
in vec2 aPosition;
in vec3 aColor;
out vec3 iColor;

void main() {
    gl_Position = vec4(aPosition.xy, 0, 1) * uMV * uP;
    iColor = aColor;
}
