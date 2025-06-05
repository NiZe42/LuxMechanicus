#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 cameraPosition;

out vec2 texCoord;
out vec3 outNormal;
out vec3 cameraPos;
out vec3 worldFragPos;
out mat4 outModelMatrix;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
    worldFragPos = vec3(modelMatrix * vec4(aPos, 1.0));
    texCoord = aTexCoord;
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    outNormal = normalMatrix * aNormal;
    outModelMatrix = modelMatrix;
    cameraPos = cameraPosition;
}