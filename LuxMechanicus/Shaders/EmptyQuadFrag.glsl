#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D quadTexture;

void main() {
    FragColor = texture(quadTexture, TexCoords);
    BrightColor = vec4(FragColor.rgb, 1.0);
}
