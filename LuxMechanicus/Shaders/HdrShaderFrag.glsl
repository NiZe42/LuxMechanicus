#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform bool useHDR;
uniform float exposure;

void main() {             
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    if(useHDR) {
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    } 
    else {
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
}

