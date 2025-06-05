#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D colorMap;
uniform sampler2D blurredBrightMap;
//uniform float exposure;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(colorMap, TexCoords).rgb;      
    vec3 bloomColor = texture(blurredBrightMap, TexCoords).rgb;
    hdrColor += bloomColor; 

    //vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

    //result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(hdrColor, 1.0);
}