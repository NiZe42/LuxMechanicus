#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "..\..\Renderer\Shader\Shader.h"
#include "..\..\Renderer\Material\Texture.h"
#include "..\Scenes\HierarchyObject.h"

class Camera : HierarchyObject
{
public:

	Camera(float movementSpeed, float mouseSensitivity);
	~Camera();

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

	void ProcessMovementInput(glm::vec3 moveDirection, float deltaTime);
	void ProcessRotationInput(float horizaontalOffset, float verticalOffset);

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;

	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 rotation);
private:
	glm::vec3 mPosition;
	glm::vec3 mRotation;
	float mMouseSensitivity;
	float mMovementSpeed;

	glm::mat3 GetRotationMatrix() const;

};

