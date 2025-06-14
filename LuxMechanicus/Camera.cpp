#include "Camera.h"

Camera::Camera(float movementSpeed, float mouseSensitivity) 
    : mMovementSpeed(movementSpeed), mMouseSensitivity(mouseSensitivity) {

    SetPosition(glm::vec3(0.0f));
    SetRotation(glm::vec3(0.0f));
    std::cout << mPosition.x << ";" << mPosition.y << ";" << mPosition.z << std::endl;
}

Camera::~Camera() {

}

glm::mat4 Camera::GetViewMatrix() const {
    glm::mat3 invRotationMatrix = glm::transpose(GetRotationMatrix());
    glm::vec3 invTranslationMatrix = -invRotationMatrix * mPosition;

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    viewMatrix[0] = glm::vec4(invRotationMatrix[0], 0.0f);
    viewMatrix[1] = glm::vec4(invRotationMatrix[1], 0.0f);
    viewMatrix[2] = glm::vec4(invRotationMatrix[2], 0.0f);
    viewMatrix[3] = glm::vec4(invTranslationMatrix, 1.0f);

    return viewMatrix;
}


// Locked Camera for now.
glm::mat4 Camera::GetProjectionMatrix() const {
    return glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
}

// Moves the camera along local axis
void Camera::ProcessMovementInput(glm::vec3 moveDirection, float deltaTime) {
    if (moveDirection == glm::vec3(0.0f))
        return;
    moveDirection = glm::normalize(moveDirection) * mMovementSpeed * deltaTime;
    mPosition += GetRotationMatrix() * moveDirection;
}

void Camera::ProcessRotationInput(float horizontalOffset, float verticalOffset) {
    mRotation.y -= horizontalOffset * mMouseSensitivity;
    mRotation.x += verticalOffset * mMouseSensitivity;
}

glm::vec3 Camera::GetPosition() const {
    return mPosition;
}

glm::vec3 Camera::GetRotation() const {
    return mRotation;
}

void Camera::SetPosition(glm::vec3 position) {
    mPosition = position;
}

void Camera::SetRotation(glm::vec3 rotation) {
    mRotation = rotation;
}

glm::mat3 Camera::GetRotationMatrix() const {
    float pitch = glm::radians(mRotation.x); 
    float yaw = glm::radians(mRotation.y);
    float roll = glm::radians(mRotation.z);

    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    rotationMatrix = glm::rotate(rotationMatrix, yaw, glm::vec3(0, 1, 0));
    rotationMatrix = glm::rotate(rotationMatrix, pitch, glm::vec3(1, 0, 0));
    rotationMatrix = glm::rotate(rotationMatrix, roll, glm::vec3(0, 0, 1));
     
    return glm::mat3(rotationMatrix);
}
