#version 410 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoords;

out vec2 passTexCoords;

uniform mat4 transform;
uniform mat4 projection;

void main() {
    passTexCoords = texCoords;
    gl_Position = projection * (transform * vec4(pos, -1, 1.0));
}