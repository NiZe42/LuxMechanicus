#pragma once
#include "LightSource.h"

class PointLight : public LightSource {
public:
    
    PointLight(glm::vec3 position,
        glm::vec3 rotation,
        glm::vec3 scale,
        const char* vertexShaderPath,
        const char* fragmentShaderPath,
        glm::vec3 color = glm::vec3(1.0f),
        float intensity = 1.0f,
        float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f);

    virtual ~PointLight() override;

    glm::vec3 GetAttenuation() const;
    void SetAttenuation(glm::vec3 attenuation);

    virtual void ApplyLightProperties() const override;

private:
    glm::vec3 pAttenuation; 
};
