#pragma once
#include <iostream>
#include <sstream>
#include "LightType.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct alignas(16) LightData {
    glm::vec3 position;
    float pad0;  // 16 bytes.
    
    glm::vec3 attenuation;
    float pad1; // 32 bytes.

    glm::vec3 color;
    float pad2; // 48 bytes.

    glm::vec3 direction;  // DIRECTIONAL and SPOT properties.
    float pad3; // 64 bytes.

    LightType lightType;
    bool castShadows; // on GPU bool is packed into 4 bytes. 
                      // Insane waste, need to manually handle it at some point.
    bool pad4[3];
    float intensity; 
    float cutoff; //SPOT properties.
    
    unsigned int lightIndex;
    float pad5[3];// 96 bytes

    static std::string ToString(const LightData& light) {
        std::ostringstream string;
        string << "LightData {\n";
        string << "  Type: (" << static_cast<uint32_t>(light.lightType) << ")\n";
        string << "  Position: (" << light.position.x << ", " << light.position.y << ", " << light.position.z << ")\n";
        string << "  Attenuation: (" << light.attenuation.x << ", " << light.attenuation.y << ", " << light.attenuation.z << ")\n";
        string << "  Intensity: (" << light.intensity << ")\n";
        string << "  Color: (" << light.color.r << ", " << light.color.g << ", " << light.color.b << ")\n";
        string << "  CastShadow: (" << light.castShadows << ")\n";
        string << "  Direction: (" << light.direction.x << ", " << light.direction.y << ", " << light.direction.z << ")\n";
        string << "  Cutoff: (" << light.cutoff << ")\n";
        string << "  lightIndex: (" << light.lightIndex << ")\n";
        string << "}";

        return string.str();
    }

    glm::mat4 GetDirectionalLightVPMatrix(float orthoSize = 15.0f, float near = 0.0f, float far = 30.0f, float offsetDistance = 10.0f) const {
        glm::vec3 sceneCenter = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 lightDirNormalized = glm::normalize(direction);

        glm::vec3 eye = glm::vec3(0.0f, 0.0f, 0.0f) - lightDirNormalized * offsetDistance;

        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        if (glm::abs(glm::dot(lightDirNormalized, up)) > 0.99f) {
            up = glm::vec3(1.0f, 0.0f, 0.0f); 
        }

        glm::mat4 lightView = glm::lookAt(eye, sceneCenter, up);

        glm::mat4 lightProjection =
            glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, near, far);

        return lightProjection * lightView;
    }

};

struct alignas(16) ShadowCasterData {
    unsigned int lightIndex;
    int shadowCasterId; // Index at which shadowmap is stored at shadow atlas.
                        // Indexes could have gaps in between them as shadow maps are designed to be removed/added.
    float pad[2];       // 16 bytes.

    glm::mat4 lightVPMatrix; // 80 bytes.

    ShadowCasterData(
        unsigned int pLightIndex, 
        int pShadowCasterId,
        glm::mat4 pLightVPMatrix) 
        : lightIndex(pLightIndex), 
          shadowCasterId(pShadowCasterId),
          pad{0.0f, 0.0f},
          lightVPMatrix(pLightVPMatrix) {
    }

    static std::string ToString(const ShadowCasterData& light) {
        std::ostringstream string;
        string << "ShadowCasterData {\n";
        string << "  lightIndex: (" << light.lightIndex << ")\n";
        string << "  Shadow Caster Id: (" << light.shadowCasterId << ")\n";
        string << "  Light View Projection Matrix: " << "\n";
            for (int row = 0; row < 4; ++row) {
                string << "[ ";
                for (int col = 0; col < 4; ++col) {
                    string << light.lightVPMatrix[col][row] << " ";
                }
                string << "]\n";
            };

        return string.str();
    }
};


