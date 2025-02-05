#pragma once
#include "LightSource.h"

class SpotLight : public LightSource {
public:
    SpotLight(glm::vec3 position,
        glm::vec3 rotation,
        glm::vec3 scale,
        const char* vertexShaderPath,
        const char* fragmentShaderPath,
        glm::vec3 color = glm::vec3(1.0f),
        float intensity = 1.0f,
        glm::vec3 direction = glm::vec3(1.0f, 1.0f, 0.0f),
        float cutoff = 20.0f);

    virtual ~SpotLight() override;

    glm::vec3 GetDirection() const;
    void SetDirection(glm::vec3 direction);

    float GetCutoff() const;
    void SetCutoff(float cutoff);

    virtual void ApplyLightProperties() const override;

private:
    glm::vec3 m_direction;
    float pCutoff;
};
