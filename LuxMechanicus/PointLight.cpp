#include "PointLight.h"
#include <iostream>

PointLight::PointLight(glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale,
    const char* vertexShaderPath,
    const char* fragmentShaderPath,
    glm::vec3 color,
    float intensity,
    float constant, float linear, float quadratic)
    : LightSource(position, rotation, scale, vertexShaderPath, fragmentShaderPath, intensity),
    pAttenuation(glm::vec3(constant, linear, quadratic)) {
    
    pColor = color;
    pIntensity = intensity;
}

PointLight::~PointLight() {}

glm::vec3 PointLight::GetAttenuation() const {
    return pAttenuation;
}

void PointLight::SetAttenuation(glm::vec3 attenuation) {
    pAttenuation = attenuation;
}

void PointLight::ApplyLightProperties() const {
    std::cout << "Applying PointLight Properties:" << std::endl;
    std::cout << "Color: " << pColor.x << ", " << pColor.y << ", " << pColor.z << std::endl;
    std::cout << "Intensity: " << pIntensity << std::endl;
    std::cout << "Attenuation: " << pAttenuation.x << ", " << pAttenuation.y << ", " << pAttenuation.z << std::endl;

    // Apply to shaders (For example):
    // glUniform3fv(glGetUniformLocation(shaderID, "pointLight.position"), 1, &pPosition[0]);
    // glUniform3fv(glGetUniformLocation(shaderID, "pointLight.color"), 1, &pColor[0]);
    // glUniform1f(glGetUniformLocation(shaderID, "pointLight.intensity"), pIntensity);
    // glUniform3fv(glGetUniformLocation(shaderID, "pointLight.attenuation"), 1, &pAttenuation[0]);
}
