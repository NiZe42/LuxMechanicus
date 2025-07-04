#pragma once

#include <vector>
#include "GLIncludes.h"
#include "LightSource.h"
#include <iostream>

class LightProcessor {
public:
    LightProcessor();
    ~LightProcessor();

    void AddLight(LightData light);
    void ClearLights();

    void UploadToGPU();              
    void BindSSBO(unsigned int binding); 

    const std::vector<LightData>& GetLights() const;

private:
    GLuint SSBOId;
    std::vector<LightData> lights;
};
