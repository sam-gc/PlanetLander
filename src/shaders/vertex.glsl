#version 140

uniform mat4 uMV;
in vec2 aPosition;

void main() {
    gl_Position = vec4(aPosition.xy, 0, 1) * uMV;
}
