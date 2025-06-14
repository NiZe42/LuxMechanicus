#version 460 core
out vec4 FragColor;
  
uniform vec3 lightColor;
uniform float lightIntensity;
uniform bool hasTexture;

void main()
{
    if(hasTexture)
        FragColor = vec4(lightColor * lightIntensity, 1.0f);
    else {
        FragColor = vec4(lightColor * lightIntensity, 1.0f);
    }
}