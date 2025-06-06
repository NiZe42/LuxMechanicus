#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D image;
  
uniform bool isHorizontal;
float offsetWeight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
// Normal distribution weights for +- 4 pixels.

void main()
{             
    vec2 tex_offset = 5.0 / textureSize(image, 0); 
    vec3 result = texture(image, TexCoords).rgb * offsetWeight[0]; 
    if(isHorizontal) {
        for (int i = 1; i < 5; ++i) {
            result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * offsetWeight[i];
            result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * offsetWeight[i];
        }
    } else {
        for (int i = 1; i < 5; ++i) {
            result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * offsetWeight[i];
            result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * offsetWeight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}