#pragma once
#include "GameObject.h"
#include <glm/glm.hpp>

class LightSource : public GameObject {
public:
    LightSource(glm::vec3 position,
        glm::vec3 rotation,
        glm::vec3 scale,
        const char* vertexShaderPath,
        const char* fragmentShaderPath);
    virtual ~LightSource() = default;

    glm::vec3 GetColor() const;
    void SetColor(glm::vec3 color);
    
    float GetIntensity() const;
    void SetIntensity(float intensity);

    virtual void ApplyLightProperties() const = 0;  // Pure virtual function for applying light properties in shaders

    virtual void Render(glm::mat4 viewMatrix, glm::mat4 projectMatrix) override;

protected:
    glm::vec3 pColor;
    float pIntensity;
};

