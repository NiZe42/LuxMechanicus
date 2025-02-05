#version 330 core
out vec4 fragColor;
  
uniform vec3 lightColor;

in vec2 TexCoord;
in vec3 outNormal;

void main()
{
    fragColor = vec4(lightColor, 1.0f);
}