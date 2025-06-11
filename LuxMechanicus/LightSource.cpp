#include "LightSource.h"
#include "Shader.h" 

LightSource::LightSource(glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale,
    const char* vertexShaderPath,
    const char* fragmentShaderPath)
    : GameObject(position, rotation, scale, vertexShaderPath, fragmentShaderPath) {
    SetPosition(position);
}

void LightSource::SetLightType(LightType type) {
    lightData.lightType = type;
}

LightType LightSource::GetLightType() const {
    return lightData.lightType;
}

glm::vec3 LightSource::GetPosition() const {
    if (mPosition != lightData.position) {
        std::cerr << "LightData and game object positions are different. Assuming game object position as correct" << std::endl;
    }
    return mPosition;
}

void LightSource::SetPosition(glm::vec3 position) {
    lightData.position = position;
    GameObject::SetPosition(position);
}

glm::vec3 LightSource::GetAttenuation() const {
    return lightData.attenuation;
}

void LightSource::SetAttenuation(glm::vec3 attenuation) {
    lightData.attenuation = attenuation;
}

float LightSource::GetIntensity() const {
    return lightData.intensity;
}

void LightSource::SetIntensity(float intensity) {
    lightData.intensity = intensity;
}

glm::vec3 LightSource::GetColor() const {
    return lightData.color;
}

void LightSource::SetColor(glm::vec3 color) {
    lightData.color = color;
}

glm::vec3 LightSource::GetDirection() const {
    return lightData.direction;
}

void LightSource::SetDirection(glm::vec3 direction) {
    lightData.direction = direction;
}

float LightSource::GetCutoff() const {
    return lightData.cutoff;
}

void LightSource::SetCutoff(float cutoff) {
    lightData.cutoff = cutoff;
}

void LightSource::SetDefaultValues(LightType lightType) {
    SetLightType(lightType);
    SetAttenuation(glm::vec3(1.0f, 0.09f, 0.032f));
    SetIntensity(1.0f);
    SetColor(glm::vec3(1.0f));

    switch (lightType) {
    case LightType::DIRECTIONAL:
        SetDirection(glm::vec3(1.0f, 0.0f, 0.0f));
        SetCutoff(0.0f);
        break;

    case LightType::POINT:
        SetDirection(glm::vec3(0.0f, 0.0f, 0.0f));
        SetCutoff(0.0f);
        break;

    case LightType::SPOT:
        SetDirection(glm::vec3(1.0f, 0.0f, 0.0f));
        SetCutoff(40.0f);
        break;
    }
}

LightData LightSource::GetLightData() {
    return lightData;
}

void LightSource::Render(glm::mat4 viewMatrix, glm::mat4 projectMatrix) {
    pShader->SetUniformFloat("lightIntensity", lightData.intensity);
    pShader->SetUniformVector3("lightColor", lightData.color);
    GameObject::Render(viewMatrix, projectMatrix);
}