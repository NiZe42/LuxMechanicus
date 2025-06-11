#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "LightSource.h"
#include <iostream>
#include "Shader.h"
#include "Environment.h"
#include "PostProcessor.h"
#include "LightProcessor.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Render(const std::vector<Scene*>& scenesToRender);

    void RenderScene(Scene* scene);

    void RenderGameObject(GameObject* object, glm::mat4 viewMatrix);

    void SetActiveCamera(Camera* camera);
    static Camera* GetActiveCamera();

    void AddLight(LightSource* lightSource);

    void SetProjectionMatrix(glm::mat4 projectionMatrix);

    void SetScreenWidth(int width);
    void SetScreenHeight(int height);

private:
    static Camera* mActiveCamera;

    int mScreenWidth, mScreenHeight;

    PostProcessor* postProcessor;
    LightProcessor* lightProcessor;

    glm::mat4 mProjectionMatrix;

    void SendCameraInfoToShader(Shader* shader);
};


