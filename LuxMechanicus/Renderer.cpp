#include "Renderer.h"
#include <iostream>

Renderer::Renderer() : mActiveCamera(nullptr) {
    glEnable(GL_DEPTH_TEST);
}

Renderer::~Renderer() {
    if (mActiveCamera != nullptr)
        delete mActiveCamera;
}

void Renderer::Render(const std::vector<Scene*>& scenesToRender) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (Scene* scene : scenesToRender) {
        RenderScene(scene);
    }
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

Camera* Renderer::GetActiveCamera() const {
    return mActiveCamera;
}

void Renderer::SetProjectionMatrix(glm::mat4 projectionMatrix) {
    mProjectionMatrix = projectionMatrix;
}