#pragma once
#include "GameObject.h"
#include <glm/glm.hpp>
#include "LightType.h"
#include "LightData.h"

class LightSource : public GameObject {
public:
    LightSource(glm::vec3 position,
        glm::vec3 rotation,
        glm::vec3 scale,
        const char* vertexShaderPath,
        const char* fragmentShaderPath);
    virtual ~LightSource() = default;

    LightType GetLightType() const;
    void SetLightType(LightType lightType);

    glm::vec3 GetPosition() const override;
    void SetPosition(glm::vec3 position) override;
    
    glm::vec3 GetAttenuation() const;
    void SetAttenuation(glm::vec3 attenuation);

    float GetIntensity() const;
    void SetIntensity(float intensity);

    glm::vec3 GetColor() const;
    void SetColor(glm::vec3 color);
    
    bool GetCastShadows() const;
    void SetCastShadows(bool castShadows);

    glm::vec3 GetDirection() const;
    void SetDirection(glm::vec3 direction);

    float GetCutoff() const;
    void SetCutoff(float cutoff);

    unsigned int GetLightIndex() const;
    void SetLightIndex(unsigned int lightIndex);

    LightData GetLightData();

    void SetDefaultValues(LightType lightType);

    void Render(glm::mat4 viewMatrix, glm::mat4 projectMatrix) override;

private:

    static unsigned int lightIndexCounter;
    LightData lightData;
};

