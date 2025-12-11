#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

layout (location = 3) in mat4 instanceModel;
//uniform mat4 modelMatrix;

void main()
{
    gl_Position = instanceModel * vec4(aPos, 1.0);
} 