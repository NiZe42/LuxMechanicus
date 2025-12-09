#include "DeferredRenderer.h"

DeferredRenderer::DeferredRenderer() {
    Initialize(1920, 1080);
}

DeferredRenderer::~DeferredRenderer() {
    if (gFrameBuffer)
        delete gFrameBuffer;
    
    if(geometryShader)
        delete geometryShader;
    
    if(lightingShader)
        delete lightingShader;
}

void DeferredRenderer::Initialize(unsigned int width, unsigned int height) {
    geometryShader = new Shader((std::string(Environment::GetRootPath()) + "/Shaders/DeferredGVert.glsl").c_str(),
        (std::string(Environment::GetRootPath()) + "/Shaders/DeferredGFrag.glsl").c_str());
    lightingShader = new Shader((std::string(Environment::GetRootPath()) + "/Shaders/DeferredLightVert.glsl").c_str(),
        (std::string(Environment::GetRootPath()) + "/Shaders/DeferredLightFrag.glsl").c_str());
    
    std::vector<RenderTextureType> gOutputTextures = {
        RenderTextureType::G_POSITION,
        RenderTextureType::G_NORMAL,
        RenderTextureType::G_ALBEDO_SPEC
    };

    gFrameBuffer = FrameBuffer::Builder::Builder()
        .WithRenderTextures(gOutputTextures)
        .WithDepthRBO()
        .Build();

    InitializeQuad();
}

void DeferredRenderer::GeometryPass(const std::vector<Scene*>& scenesToRender, const Camera& camera) {
    gFrameBuffer->Bind();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    geometryShader->Bind();
    geometryShader->SetUniformMat4("viewMatrix", camera.GetViewMatrix());
    geometryShader->SetUniformMat4("projectionMatrix", camera.GetProjectionMatrix());

    for (Scene* scene : scenesToRender) {
        for (HierarchyObject* child : scene->GetChildren()) {
            GameObject* object = dynamic_cast<GameObject*>(child);
            if (object && object->GetRenderingType() != RenderingType::FORWARD_RENDERING) {
                RenderGameObject(object);
            }
        }
    }

    geometryShader->Unbind();
    gFrameBuffer->Unbind();
}

void DeferredRenderer::LightingPass(const Camera& camera, unsigned int DepthLayeredTextureArrayId) {
    lightingShader->Bind();
    lightingShader->SetUniformVector3("cameraPosition", camera.GetPosition());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gFrameBuffer->GetAttachedRenderTextureIdByType(RenderTextureType::G_POSITION));
    lightingShader->SetUniformInt("gPosition", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gFrameBuffer->GetAttachedRenderTextureIdByType(RenderTextureType::G_NORMAL));
    lightingShader->SetUniformInt("gNormal", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gFrameBuffer->GetAttachedRenderTextureIdByType(RenderTextureType::G_ALBEDO_SPEC));
    lightingShader->SetUniformInt("gAlbedoSpec", 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D_ARRAY, DepthLayeredTextureArrayId);
    lightingShader->SetUniformInt("shadowAtlasArray", 3);

    RenderFullscreenQuad();

    lightingShader->Unbind();
}

void DeferredRenderer::RenderGameObject(GameObject* object) {
    //std::cout << "Rendering gameobject" << object->GetGameObjectId() << std::endl;

    object->DeferredRender(geometryShader);

    for (HierarchyObject* child : object->GetChildren()) {
        GameObject* childObject = dynamic_cast<GameObject*>(child);
        if (childObject && childObject->GetRenderingType() != RenderingType::FORWARD_RENDERING) {
            RenderGameObject(childObject);
        }
    }
}

void DeferredRenderer::InitializeQuad() {
    float quadVertices[] = {
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    glGenVertexArrays(1, &quadVAOId);

    glGenBuffers(1, &quadVBOId);
    glBindVertexArray(quadVAOId);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBOId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void DeferredRenderer::RenderFullscreenQuad() {
    glBindVertexArray(quadVAOId);
    Profiler::currentDrawCalls++;
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

FrameBuffer* DeferredRenderer::GetGFrameBuffer() {
    return gFrameBuffer;
}

