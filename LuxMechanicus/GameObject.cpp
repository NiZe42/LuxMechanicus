#include "GameObject.h"
#include "Shader.h"
#include "Texture.h"

unsigned int GameObject::nextGameObjectId = 0;  

GameObject::GameObject(
    glm::vec3 position, 
    glm::vec3 rotation, 
    glm::vec3 scale,
    const char* vertexShaderPath, 
    const char* fragmentShaderPath, 
    const char* texturePath) 
    : mPosition(position), mRotation(rotation), mScale(scale) {
    if (nextGameObjectId == 0)
        nextGameObjectId++;

    mGameObjectId = nextGameObjectId++;

    InitializeShader(vertexShaderPath, fragmentShaderPath);

    if (texturePath)
        InitializeTexture(texturePath);
}

GameObject::~GameObject() {
    delete pShader;
    delete pTexture;
    delete pMesh;
}

unsigned int GameObject::GetGameObjectId() const {
    return mGameObjectId;
}

glm::vec3 GameObject::GetPosition() const {
    return mPosition;
}

glm::vec3 GameObject::GetRotation() const {
    return mRotation;
}

glm::vec3 GameObject::GetScale() const {
    return mScale;
}

glm::mat4 GameObject::GetModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);  

    model = glm::translate(model, mPosition);

    // Would suffer from Gimbal Lock, would need to use quats instead
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(mRotation.x), glm::vec3(1, 0, 0));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(mRotation.y), glm::vec3(0, 1, 0));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(mRotation.z), glm::vec3(0, 0, 1));

    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), mScale);

    model = model * rotationMatrix * scaleMatrix;

    return model;
}

void GameObject::SetMesh(Mesh* mesh) {
    if (pMesh != nullptr)
        delete pMesh;
    pMesh = mesh;
}

void GameObject::SetShader(Shader* shader) {
    if (pShader != nullptr)
        delete pShader;
    pShader = shader;
}

void GameObject::SetTexture(Texture* texture) {
    if (pTexture != nullptr)
        delete pTexture;
    pTexture = texture;
}

void GameObject::SetPosition(glm::vec3 position) {
    mPosition = position;
}

void GameObject::SetRotation(glm::vec3 rotation) {
    mRotation = rotation;
}

void GameObject::SetScale(glm::vec3 scale) {
    mScale = scale;
}

void GameObject::InitializeShader(const char* vertexShaderPath, 
    const char* fragmentShaderPath) {
    pShader = new Shader(vertexShaderPath, fragmentShaderPath);
}

void GameObject::InitializeTexture(const char* texturePath) {
    pTexture = new Texture(texturePath);
}


void GameObject::InitializeSquare() {
    float vertices[] = {
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &vaoId);
    glGenBuffers(1, &vboId);
    glGenBuffers(1, &eboId);

    glBindVertexArray(vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void GameObject::InitializeCube() {
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &vaoId);
    glGenBuffers(1, &vboId);

    glBindVertexArray(vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

// Should change the name to ForwardRender.
void GameObject::Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
    if (pShader && pMesh) {
        
        if (pTexture) {
            glActiveTexture(GL_TEXTURE0);
            pTexture->Bind();
            pShader->SetUniformInt("ourTexture", 0);
            pShader->SetUniformBool("hasTexture", true);

        } else {
            pShader->SetUniformBool("hasTexture", false);
        }

        pShader->SetUniformMat4("modelMatrix", GetModelMatrix());
        pShader->SetUniformMat4("viewMatrix", viewMatrix);
        pShader->SetUniformMat4("projectionMatrix", projectionMatrix);

        pMesh->Render();
    }
}

void GameObject::DeferredRender(Shader* gShader) {
    gShader->SetUniformMat4("modelMatrix", GetModelMatrix());

    glActiveTexture(GL_TEXTURE0);
    if(pTexture)
        pTexture->Bind();
    gShader->SetUniformInt("albedoMap", 0);

    pMesh->Render();
}

void GameObject::SetRenderingType(RenderingType pRenderingType) {
    renderingType = pRenderingType;
}

RenderingType GameObject::GetRenderingType() const {
    return renderingType;
}


Shader* GameObject::GetShader() const{
    return pShader;
}

void GameObject::BindShader() {
    if (pShader)
        pShader->Bind();
}