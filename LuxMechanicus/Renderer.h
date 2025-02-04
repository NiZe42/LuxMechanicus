#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Render(const std::vector<Scene*>& scenesToRender);

    void RenderScene(Scene* scene);

    void RenderGameObject(GameObject* object, glm::mat4 viewMatrix);

    void SetActiveCamera(Camera* camera);
    Camera* GetActiveCamera() const;

    void SetProjectionMatrix(glm::mat4 projectionMatrix);

private:
    Camera* mActiveCamera;

    glm::mat4 mProjectionMatrix;
};


