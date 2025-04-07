#version 330 core
out vec4 fragColor;
  
uniform vec3 lightColor;
uniform bool hasTexture;

in vec2 TexCoord;
in vec3 outNormal;

void main()
{
    //Need this not to throw debug warnings
    if(!hasTexture){
    fragColor = vec4(lightColor, 1.0f);
    } else {
    fragColor = vec4(lightColor, 1.0f);
    }
}