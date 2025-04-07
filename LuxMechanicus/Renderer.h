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
#include "PointLight.h"
#include "SpotLight.h"
#include "Shader.h"
#include "Environment.h"


class Renderer {
public:
    Renderer();
    ~Renderer();

    void Render(const std::vector<Scene*>& scenesToRender);

    void RenderScene(Scene* scene);

    void RenderGameObject(GameObject* object, glm::mat4 viewMatrix);

    void SetActiveCamera(Camera* camera);
    static Camera* GetActiveCamera();

    void AddLight(LightSource*lightSource);
    static std::vector<LightSource*> GetAllLightSources();

    void SetProjectionMatrix(glm::mat4 projectionMatrix);

    void SetScreenWidth(int width);
    void SetScreenHeight(int height);

private:
    static Camera* mActiveCamera;
    static std::vector<LightSource*> mAllLights;

    int mScreenWidth, mScreenHeight;

    Shader* hdrShader;
    unsigned int hdrFBOId, colorBufferId;
    bool useHDR;

    unsigned int quadVaoId, quadVboId;

    glm::mat4 mProjectionMatrix;

    void SendLightInfoToShader(Shader* shader);
    void SendCameraInfoToShader(Shader* shader);
    void InitializeHDR();
    void InitializeScreenQuad();
    void RenderScreenQuad();
};


