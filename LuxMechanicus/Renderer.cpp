#include "Renderer.h"
#include <iostream>

Camera* Renderer::mActiveCamera = nullptr;
std::vector<LightSource*> Renderer::mAllLights;

Renderer::Renderer() {
    glEnable(GL_DEPTH_TEST);

    postProcessor = new PostProcessor();
}

Renderer::~Renderer() {
    if (mActiveCamera)
        delete mActiveCamera;

    if (postProcessor)
        delete postProcessor;
}

void Renderer::Render(const std::vector<Scene*>& scenesToRender) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    postProcessor->BindFirstFrameBuffer();

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

    if (!dynamic_cast<PointLight*>(object) && !dynamic_cast<SpotLight*>(object)) {
    SendLightInfoToShader(shader);
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
    mAllLights.push_back(lightSource);
}

std::vector<LightSource*> Renderer::GetAllLightSources() {
    return mAllLights;
}

void Renderer::SendLightInfoToShader(Shader* shader) {
    std::vector<LightSource*> allLights = Renderer::GetAllLightSources();

    GLuint shaderProgramId = shader->GetShaderProgramId();

    std::vector<glm::vec3> pointLightColors;
    std::vector<glm::vec3> pointLightPositions;
    std::vector<glm::vec3> pointLightAttenuations;
    std::vector<float> pointLightIntensities;

    std::vector<glm::vec3> spotLightColors;
    std::vector<glm::vec3> spotLightPositions;
    std::vector<glm::vec3> spotLightDirections;
    std::vector<float> spotLightCutoffs;
    std::vector<float> spotLightIntensities;

    int pointLightCount = 0;
    int spotLightCount = 0;

    for (LightSource* light : allLights) {
        if (auto pointLight = dynamic_cast<PointLight*>(light)) {
            pointLightColors.emplace_back(pointLight->GetColor());
            pointLightPositions.emplace_back(pointLight->GetPosition());
            pointLightAttenuations.emplace_back(pointLight->GetAttenuation());
            pointLightIntensities.emplace_back(pointLight->GetIntensity());
            pointLightCount++;
        }
        else if (auto spotLight = dynamic_cast<SpotLight*>(light)) {
            spotLightColors.emplace_back(spotLight->GetColor());
            spotLightPositions.emplace_back(spotLight->GetPosition());
            spotLightDirections.emplace_back(spotLight->GetDirection());
            spotLightCutoffs.push_back(spotLight->GetCutoff());
            spotLightIntensities.emplace_back(spotLight->GetIntensity());
            spotLightCount++;
        }
    }

    shader->SetUniformInt("pointLightCount", pointLightCount);
    shader->SetUniformInt("spotLightCount", spotLightCount);

    //point light info
    shader->SetUniformVectorList("pointLightColors", pointLightColors);
    shader->SetUniformVectorList("pointLightPositions", pointLightPositions);
    shader->SetUniformVectorList("pointLightAttenuations", pointLightAttenuations);
    shader->SetUniformFloatList("pointLightIntensities", pointLightIntensities);

    // spotlight info
    shader->SetUniformVectorList("spotLightColors", spotLightColors);
    shader->SetUniformVectorList("spotLightPositions", spotLightPositions);
    shader->SetUniformVectorList("spotLightDirections", spotLightDirections);
    shader->SetUniformFloatList("spotLightCutoffs", spotLightCutoffs);
    shader->SetUniformFloatList("spotLightIntensities", spotLightIntensities);
}

void Renderer::SendCameraInfoToShader(Shader* shader) {
    glm::vec3 cameraPosition = Renderer::GetActiveCamera()->GetPosition();
    shader->SetUniformVector("cameraPosition", cameraPosition);
}

void Renderer::SetScreenHeight(int height) {
    mScreenHeight = height;
}

void Renderer::SetScreenWidth(int width) {
    mScreenWidth = width;
}