#include "GameObject.h"
#include "Shader.h"
#include "Texture.h"

unsigned int GameObject::nextGameObjectId = 0;  

GameObject::GameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) 
    : mPosition(position), mRotation(rotation), mScale(scale) {
    if (nextGameObjectId == 0)
        nextGameObjectId++;

    mGameObjectId = nextGameObjectId++;

    InitializeRenderInformation();
}

GameObject::~GameObject() {
    delete mShader;
    delete mTexture;
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

glm::mat4 GameObject::GetViewMatrix() const {
    glm::mat4 view = glm::mat4(1.0f);
    return view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
}

glm::mat4 GameObject::GetProjectionMatrix() const {
    glm::mat4 projection = glm::mat4(1.0f);
    return projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}

void GameObject::SetShader(Shader* shader) {
    if (mShader != nullptr)
        delete mShader;
    mShader = shader;
}

void GameObject::SetTexture(Texture* texture) {
    if (mTexture != nullptr)
        delete mTexture;
    mTexture = texture;
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

void GameObject::InitializeRenderInformation() {
    mShader = new Shader("C:/MyProjects/MyEngine/3DEngine/LuxMechanicus/Shaders/ColorVert.glsl", "C:/MyProjects/MyEngine/3DEngine/LuxMechanicus/Shaders/ColorFrag.glsl");
    mTexture = new Texture("C:/MyProjects/MyEngine/3DEngine/LuxMechanicus/Textures/MinecraftDirtTexture.jpg ");

    InitializeCube();
    //InitializeSquare();
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

void GameObject::Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
    if (mShader && mTexture) {
        //std::cout << "rendering" << std::endl;
        mShader->Bind();
        
        //std::cout << mTexture->GetTextureId() << std::endl;
        mTexture->Bind();
        mShader->SetUniformInt("ourTexture", 0);

        mRotation.z += 0.1f;
        if (mRotation.z >= 360)
            mRotation.z = 0;

        glm::mat4 modelMatrix = GetModelMatrix();

        mShader->SetUniformMat4("modelMatrix", modelMatrix);
        mShader->SetUniformMat4("viewMatrix", viewMatrix);
        mShader->SetUniformMat4("projectionMatrix", projectionMatrix);

        glBindVertexArray(vaoId);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}