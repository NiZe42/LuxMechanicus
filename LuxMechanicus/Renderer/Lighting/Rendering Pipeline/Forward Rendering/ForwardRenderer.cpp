#include "ForwardRenderer.h"

ForwardRenderer::ForwardRenderer() {

}

ForwardRenderer::~ForwardRenderer() {
    
}

void ForwardRenderer::Initialize() {
    
}

void ForwardRenderer::RenderForwardGameObjectList(const Camera& camera) {
    for (int i = 0; i < gameObjectList.size(); i++) {

        gameObjectList[i]->BindShader();

        gameObjectList[i]->Render(camera.GetViewMatrix(), camera.GetProjectionMatrix());
    }
}

void ForwardRenderer::AddGameObject(GameObject* gameObject) {
    gameObjectList.emplace_back(gameObject);
    gameObject->SetRenderingType(RenderingType::FORWARD_RENDERING);
}
