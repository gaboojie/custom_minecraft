#version 410 core
layout (location = 0) in uint data;

flat out uint passBlockTexID;
out vec4 passColor;
out vec3 passNormal;
out vec3 passPos;

const int NUM_BLOCKS = 11;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
uniform uint blockTexIDs[NUM_BLOCKS * 6];

void main() {
    uint block = (0xFF000000u & data) >> 24;
    uint normal = (0x00FC0000u & data) >> 18;
    uint vertexData = (0x0003FFFFu & data);

    passNormal = (normal & 1u) * vec3(0, 1, 0);

    uint texOffset = ((normal & 2u) >> 1) * 1;
    passNormal += ((normal & 2u) >> 1) * vec3(0, -1, 0);

    texOffset += ((normal & 4u) >> 2) * 2;
    passNormal += ((normal & 4u) >> 2) * vec3(-1, 0, 0);

    texOffset += ((normal & 8u) >> 3) * 3;
    passNormal += ((normal & 8u) >> 3) * vec3(1, 0, 0);

    texOffset += ((normal & 16u) >> 4) * 4;
    passNormal += ((normal & 16u) >> 4) * vec3(0, 0, -1);

    texOffset += ((normal & 32u) >> 5) * 5;
    passNormal += ((normal & 32u) >> 5) * vec3(0, 0, 1);

    passBlockTexID = blockTexIDs[block*6 + texOffset];
    vec3 vertexPos = vec3((vertexData & 0x0003F000u) >> 12, (vertexData & 0x00000FC0u) >> 6, (vertexData & 0x0000003Fu));
    gl_Position = projection * (view * (transform * vec4(vertexPos, 1.0)));
    passPos = vertexPos;
}