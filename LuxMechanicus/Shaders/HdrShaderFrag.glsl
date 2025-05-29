#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D ColorMap;
uniform bool useHDR;
uniform float exposure;

void main() {             
    const float gamma = 2.2;
    const vec3 luminanceCoefficients = vec3(0.2126, 0.7152, 0.0722);
    vec3 hdrColor = texture(ColorMap, TexCoords).rgb;
    if(useHDR) {
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    } 
    else {
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }

    float luminance = dot(FragColor.rgb, luminanceCoefficients);

    if(luminance >= 1.0){
        BrightColor = vec4(FragColor.rgb, 1.0);
    }
    else {
        BrightColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
}

