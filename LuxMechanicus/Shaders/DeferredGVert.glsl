#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 texCoords;
out vec3 normal;
out vec3 worldFragPos;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
    worldFragPos = vec3(modelMatrix * vec4(aPos, 1.0));
    texCoords = aTexCoord;
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    normal = normalMatrix * aNormal;
}