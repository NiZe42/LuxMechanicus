#pragma once

struct /*alignas(16) */ LightData {
    glm::vec3 position;
    float pad0;  // 16 bytes.
    
    glm::vec3 attenuation;
    float pad1; // 32 bytes.

    glm::vec3 color;
    float pad2; // 48 bytes.

    glm::vec3 direction;  // DIRECTIONAL and SPOT properties.
    float pad3; // 64 bytes.

    LightType lightType;
    float intensity; 
    float cutoff; //SPOT properties.
    float pad4; // 80 bytes.
  
    static std::string ToString(const LightData& light) {
        std::ostringstream string;
        string << "LightData {\n";
        string << "  Type: " << static_cast<uint32_t>(light.lightType) << "\n";
        string << "  Position: (" << light.position.x << ", " << light.position.y << ", " << light.position.z << ")\n";
        string << "  Attenuation: (" << light.attenuation.x << ", " << light.attenuation.y << ", " << light.attenuation.z << ")\n";
        string << "  Intensity: " << light.intensity << "\n";
        string << "  Color: (" << light.color.r << ", " << light.color.g << ", " << light.color.b << ")\n";
        string << "  Direction: (" << light.direction.x << ", " << light.direction.y << ", " << light.direction.z << ")\n";
        string << "  Cutoff: " << light.cutoff << "\n";
        string << "}";

        return string.str();
    }
};

