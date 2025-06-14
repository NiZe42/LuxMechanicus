#version 460 core

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

in vec2 texCoords;
in vec3 normal;
in vec3 worldFragPos;
in mat4 outModelMatrix;

uniform sampler2D albedoMap;

void main() {
    gPosition = worldFragPos;
    
    gNormal = normalize(normal);

    vec3 albedo = texture(albedoMap, texCoords).rgb;
    gAlbedoSpec = vec4(albedo, 1);
}
