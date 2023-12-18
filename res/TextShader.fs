#version 410 core

out vec4 fragColor;

in vec2 passTexCoords;

uniform vec4 color;
uniform sampler2D mappingTexture;

void main() {
    fragColor = color * texture(mappingTexture, passTexCoords);
}