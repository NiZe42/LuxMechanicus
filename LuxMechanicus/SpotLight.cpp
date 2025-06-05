#include "SpotLight.h"
#include <iostream>

SpotLight::SpotLight(glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale,
    const char* vertexShaderPath,
    const char* fragmentShaderPath,
    glm::vec3 color,
    float intensity,
    glm::vec3 direction,
    float cutoff)
    : LightSource(position, rotation, scale, vertexShaderPath, fragmentShaderPath, intensity),
    m_direction(direction), pCutoff(cutoff) {

    pColor = color;
    pIntensity = intensity;
}

SpotLight::~SpotLight() {}

glm::vec3 SpotLight::GetDirection() const {
    return m_direction;
}

void SpotLight::SetDirection(glm::vec3 direction) {
    m_direction = direction;
}

float SpotLight::GetCutoff() const {
    return pCutoff;
}

void SpotLight::SetCutoff(float cutoff) {
    pCutoff = cutoff;
}

void SpotLight::ApplyLightProperties() const {
    std::cout << "Applying SpotLight Properties:" << std::endl;
    std::cout << "Color: " << pColor.x << ", " << pColor.y << ", " << pColor.z << std::endl;
    std::cout << "Intensity: " << pIntensity << std::endl;
    std::cout << "Direction: " << m_direction.x << ", " << m_direction.y << ", " << m_direction.z << std::endl;
   
    // Apply to shaders (For example):
    // glUniform3fv(glGetUniformLocation(shaderID, "spotLight.position"), 1, &pPosition[0]);
    // glUniform3fv(glGetUniformLocation(shaderID, "spotLight.color"), 1, &pColor[0]);
    // glUniform1f(glGetUniformLocation(shaderID, "spotLight.intensity"), pIntensity);
    // glUniform3fv(glGetUniformLocation(shaderID, "spotLight.direction"), 1, &m_direction[0]);
    // glUniform1f(glGetUniformLocation(shaderID, "spotLight.cutoff"), glm::cos(glm::radians(pCutoff)));
    // glUniform1f(glGetUniformLocation(shaderID, "spotLight.outerCutoff"), glm::cos(glm::radians(m_outerCutoff)));
}
