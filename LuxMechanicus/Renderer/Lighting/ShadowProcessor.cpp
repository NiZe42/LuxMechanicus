#include "ShadowProcessor.h"
#include <iostream>

ShadowProcessor::ShadowProcessor() {
    glGenBuffers(1, &SSBOId);
    shadowPassShader = new Shader((std::string(Environment::GetRootPath()) + "/Shaders/ShadowPassVert.glsl").c_str(),
        (std::string(Environment::GetRootPath()) + "/Shaders/ShadowPassGeom.glsl").c_str(),
        (std::string(Environment::GetRootPath()) + "/Shaders/ShadowPassFrag.glsl").c_str());
}

ShadowProcessor::~ShadowProcessor() {
    if (shadowPassFrameBuffer) delete shadowPassFrameBuffer;
    if (shadowPassShader) delete shadowPassShader;
    glDeleteBuffers(1, &SSBOId);
}

void ShadowProcessor::ShadowPass(
    const std::vector<Scene*>& scenesToRender,
    MeshVaoProcessor* meshVaoProcessor) {

    BindAtlasFramebuffer();
    glViewport(0, 0, 4096, 4096);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    
    shadowPassShader->Bind();
    
    glBindVertexArray(meshVaoProcessor->GetVaoId());
    RenderScene(scenesToRender[0]);
    glBindVertexArray(0);

    shadowPassShader->Unbind();
    UnbindAtlasFramebuffer();
    glViewport(0, 0, 1920, 1080);
    glCullFace(GL_BACK);
}

void ShadowProcessor::RenderScene(Scene* scene) {
    for (HierarchyObject* child : scene->GetChildren()) {
        GameObject* object = dynamic_cast<GameObject*>(child);
        if (object && object->GetRenderingType() != RenderingType::FORWARD_RENDERING) {
            RenderGameObject(object);
        }
    }
}

void ShadowProcessor::RenderGameObject(GameObject* object) {
    //std::cout << "Rendering gameobject" << object->GetGameObjectId() << std::endl;

    object->ShadowRender(shadowPassShader);

    for (HierarchyObject* child : object->GetChildren()) {
        GameObject* childObject = dynamic_cast<GameObject*>(child);
        if (childObject && childObject->GetRenderingType() != RenderingType::FORWARD_RENDERING) {
            RenderGameObject(childObject);
        }
    }
}

void ShadowProcessor::AllocateShadowAtlases(const std::vector<LightData>& lights) {
    currentShadowCasters.clear();
    if (shadowPassFrameBuffer) delete shadowPassFrameBuffer;

    int shadowIndex = 0;
    for (const auto& light : lights) {
        if (!light.castShadows) continue;

        ShadowCasterData data = ShadowCasterData(
            light.lightIndex, 
            shadowIndex, 
            light.GetDirectionalLightVPMatrix());

        shadowIndex++;

        currentShadowCasters.emplace(light.lightIndex, data);
    }

    int shadowCastersCount = currentShadowCasters.size();

    int layerCount = shadowCastersCount / SHADOWS_PER_ATLAS;
    int moduloLayerCount = shadowCastersCount % SHADOWS_PER_ATLAS;

    std::cout << "shadow casters: " << shadowCastersCount << std::endl;
    std::cout << "layer count: " << layerCount << std::endl;

    if (moduloLayerCount != 0)
        layerCount += 1;

    // Create framebuffer that will attach different layers for rendering
    shadowPassFrameBuffer = FrameBuffer::Builder::Builder()
        .WithDepthTextureArray(layerCount, ATLAS_RES, ATLAS_RES)
        .Build();
}

void ShadowProcessor::UploadShadowCastersToGPU() {
    std::vector<ShadowCasterData> shadowCasters;
    shadowCasters.reserve(currentShadowCasters.size());

    for (const auto& pair : currentShadowCasters) {
        shadowCasters.push_back(pair.second);
        std::cout << ShadowCasterData::ToString(pair.second) << std::endl;
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOId);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(ShadowCasterData) * shadowCasters.size(), shadowCasters.data(), GL_DYNAMIC_DRAW);
    //glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ShadowProcessor::BindSSBO(unsigned int binding) {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, SSBOId);
}

void ShadowProcessor::BindAtlasFramebuffer() {
    shadowPassFrameBuffer->Bind();
    //glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);
}

void ShadowProcessor::UnbindAtlasFramebuffer() {
    shadowPassFrameBuffer->Unbind();
}

FrameBuffer* ShadowProcessor::GetFrameBuffer() const {
    return shadowPassFrameBuffer;
}

glm::ivec2 ShadowProcessor::GetAtlasSlot(int shadowIndex) const {
    int row = shadowIndex / (ATLAS_RES / SHADOW_MAP_RES);
    int col = shadowIndex % (ATLAS_RES / SHADOW_MAP_RES);
    return glm::ivec2(col * SHADOW_MAP_RES, row * SHADOW_MAP_RES);
}
