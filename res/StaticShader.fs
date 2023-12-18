#version 410 core

out vec4 color;

flat in uint passBlockTexID;
in vec3 passNormal;
in vec3 passPos;

uniform sampler2DArray mappingTexture;

void main() {
    vec2 perp = abs(passNormal.y) * vec2(passPos.x, passPos.z);
    perp += abs(passNormal.x) * vec2(passPos.z, passPos.y);
    perp += abs(passNormal.z) * vec2(passPos.x, passPos.y);
    perp = fract(perp);
    color = texture(mappingTexture, vec3(perp, passBlockTexID));
}