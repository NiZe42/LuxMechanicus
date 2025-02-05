#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "HierarchyObject.h"
#include "Mesh.h"

class GameObject : public HierarchyObject
{
public:
	GameObject(glm::vec3 position, 
		glm::vec3 rotation, 
		glm::vec3 scale,
		const char* vertexShaderPath,
		const char* fragmentShaderPath,
		const char* texturePath = nullptr);
	~GameObject();

	unsigned int GetGameObjectId() const;

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;

	Shader* GetShader() const;

	glm::mat4 GetModelMatrix() const;
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

	void SetMesh(Mesh* mesh);
	void SetShader(Shader* shader);
	void SetTexture(Texture* texture);

	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 rotation);
	void SetScale(glm::vec3 scale);

	virtual void Render(glm::mat4 viewMatrix, glm::mat4 projectMatrix);
	virtual void BindShader();
protected:
	unsigned int mGameObjectId;
	static unsigned int nextGameObjectId;

	unsigned int vaoId, vboId, eboId;

	Mesh* pMesh;
	Shader* pShader;
	Texture* pTexture;

	glm::vec3 mPosition;
	glm::vec3 mRotation;
	glm::vec3 mScale;

private:

	void InitializeShader(const char* vertexShaderPath,
		const char* fragmentShaderPath);
	void InitializeTexture(const char* texturePath);
	void InitializeSquare();
	void InitializeCube();
};

