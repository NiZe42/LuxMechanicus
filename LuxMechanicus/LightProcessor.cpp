#include "LightProcessor.h"
#include "LightData.h"
// For now cannot update Light Info during play
LightProcessor::LightProcessor() {
    glGenBuffers(1, &SSBOId);
}

LightProcessor::~LightProcessor() {
    glDeleteBuffers(1, &SSBOId);
}

void LightProcessor::AddLight(LightData light) {
    lights.push_back(light);
}

void LightProcessor::ClearLights() {
    lights.clear();
}

const std::vector<LightData>& LightProcessor::GetLights() const {
    return lights;
}

void LightProcessor::UploadToGPU() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOId);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(LightData) * lights.size(), lights.data(), GL_DYNAMIC_DRAW);
    //glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    /*for (int i = 0; i < lights.size(); i++) {
        std::cout << LightData::ToString(lights[i]) << std::endl;
    }*/
}

void LightProcessor::BindSSBO(unsigned int binding) {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, SSBOId);
}
