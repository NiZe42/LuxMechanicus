#include "Renderer.h"
#include <iostream>

Camera* Renderer::mActiveCamera = nullptr;
std::vector<LightSource*> Renderer::mAllLights;

Renderer::Renderer() : useHDR(true) {
    glEnable(GL_DEPTH_TEST);
    InitializeScreenQuad();
    InitializeHDR();
}

Renderer::~Renderer() {
    if (mActiveCamera)
        delete mActiveCamera;

    if (hdrShader)
        delete hdrShader;
}

void Renderer::Render(const std::vector<Scene*>& scenesToRender) {
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBOId);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (Scene* scene : scenesToRender) {
        RenderScene(scene);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    hdrShader->Bind();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, colorBufferId);

    hdrShader->SetUniformBool("useHDR", useHDR);
    hdrShader->SetUniformFloat("exposure", 0.5f);

    RenderScreenQuad();
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

void Renderer::InitializeHDR() {
    glGenFramebuffers(1, &hdrFBOId);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBOId);
 
    glGenTextures(1, &colorBufferId);
    glBindTexture(GL_TEXTURE_2D, colorBufferId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1920, 1080, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    unsigned int rboDepthId;
    glGenRenderbuffers(1, &rboDepthId);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepthId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferId, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepthId);

    // Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is incomplete!" << std::endl;
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        switch (status) {
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            std::cerr << "Framebuffer incomplete attachment" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            std::cerr << "Framebuffer missing attachment" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            std::cerr << "Framebuffer incomplete draw buffer" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            std::cerr << "Framebuffer incomplete read buffer" << std::endl;
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            std::cerr << "Framebuffer unsupported" << std::endl;
            break;
        default:
            std::cerr << "Framebuffer error: " << status << std::endl;
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    hdrShader = new Shader(
        (std::string(Environment::GetRootPath()) + "/Shaders/HdrShaderVert.glsl").c_str(),
        (std::string(Environment::GetRootPath()) + "/Shaders/HdrShaderFrag.glsl").c_str());

    hdrShader->Bind();
    hdrShader->SetUniformInt("hdrBuffer", 1);
}


void Renderer::InitializeScreenQuad() {
    float quadVertices[] = {
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
   
    glGenVertexArrays(1, &quadVaoId);
    
    glGenBuffers(1, &quadVboId);
    glBindVertexArray(quadVaoId);

    glBindBuffer(GL_ARRAY_BUFFER, quadVboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Renderer::SetScreenHeight(int height) {
    mScreenHeight = height;
}

void Renderer::SetScreenWidth(int width) {
    mScreenWidth = width;
}

void Renderer::RenderScreenQuad() {
    glBindVertexArray(quadVaoId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}