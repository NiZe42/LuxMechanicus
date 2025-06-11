#include "Renderer.h"
#include <iostream>

Camera* Renderer::mActiveCamera = nullptr;

Renderer::Renderer() {
    glEnable(GL_DEPTH_TEST);

    postProcessor = new PostProcessor();
    lightProcessor = new LightProcessor();
}

Renderer::~Renderer() {
    if (mActiveCamera)
        delete mActiveCamera;

    if (postProcessor)
        delete postProcessor;

    if (lightProcessor)
        delete lightProcessor;
}

void Renderer::Render(const std::vector<Scene*>& scenesToRender) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    postProcessor->BindFirstFrameBuffer();

    lightProcessor->UploadToGPU();
    lightProcessor->BindSSBO(1);

    for (Scene* scene : scenesToRender) {
        RenderScene(scene);
    }
    
    postProcessor->UnbindFirstFrameBuffer();
    
    postProcessor->ApplyEffectsAndRender();
}

void Renderer::RenderScene(Scene* scene) {
    glm::mat4 cameraViewMatrix = GetActiveCamera()->GetViewMatrix();
    for (HierarchyObject* child : scene->GetChildren()) {
        GameObject* object = dynamic_cast<GameObject*>(child);
        if (object) {
            RenderGameObject(object, cameraViewMatrix);
        }
    }
}

void Renderer::RenderGameObject(GameObject* object, glm::mat4 viewMatrix) {
    //std::cout << "Rendering gameobject" << object->GetGameObjectId() << std::endl;
    
    object->BindShader();
    
    Shader* shader = object->GetShader();

    if (!dynamic_cast<LightSource*>(object)) {
        SendCameraInfoToShader(shader);
    }
    
    object->Render(viewMatrix, mProjectionMatrix);

    for (HierarchyObject* child : object->GetChildren()) {
        GameObject* childObject = dynamic_cast<GameObject*>(child);
        if (childObject) {
            RenderGameObject(childObject, viewMatrix);
        }
    }
}

void Renderer::SetActiveCamera(Camera* camera) {
    if(mActiveCamera != nullptr)
        delete mActiveCamera;
    mActiveCamera = camera;
}

Camera* Renderer::GetActiveCamera() {
    return mActiveCamera;
}

void Renderer::SetProjectionMatrix(glm::mat4 projectionMatrix) {
    mProjectionMatrix = projectionMatrix;
}

void Renderer::AddLight(LightSource* lightSource) {
    lightProcessor->AddLight(lightSource->GetLightData());
}

void Renderer::SendCameraInfoToShader(Shader* shader) {
    glm::vec3 cameraPosition = Renderer::GetActiveCamera()->GetPosition();
    shader->SetUniformVector3("cameraPosition", cameraPosition);
}

void Renderer::SetScreenHeight(int height) {
    mScreenHeight = height;
}

void Renderer::SetScreenWidth(int width) {
    mScreenWidth = width;
}