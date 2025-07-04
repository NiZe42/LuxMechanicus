#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 modelMatrix;

struct ShadowCasterData {
    uint lightIndex;
    int shadowCasterId;
    float pad0;
    float pad1;

    mat4 lightVPMatrix;
};

layout(std430, binding = 2) buffer shadowBuffer {
    ShadowCasterData shadowCasters[];
};

void main()
{
    gl_Position = modelMatrix * vec4(aPos, 1.0);
} 