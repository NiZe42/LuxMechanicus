#pragma once
#include "..\..\Utils\GLIncludes.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "..\..\Renderer\Shader\Shader.h"
#include "..\..\Renderer\Material\Texture.h"
#include "..\Scenes\HierarchyObject.h"
#include "..\..\Renderer\Mesh\Mesh.h"
#include "..\..\Renderer\Lighting\RenderingPipeline\RenderingType.h"

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

	size_t meshMergeIndexOffset;
	size_t meshMergeIndexCount;

	unsigned int GetGameObjectId() const;

	virtual glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;

	Shader* GetShader() const;

	glm::mat4 GetModelMatrix() const;

	Mesh* GetMesh() const;

	void SetMesh(Mesh* mesh);
	void SetShader(Shader* shader);
	void SetTexture(Texture* texture);

	virtual void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 rotation);
	void SetScale(glm::vec3 scale);

	void SetRenderingType(RenderingType renderingType);
	RenderingType GetRenderingType() const;

	virtual void Render(glm::mat4 viewMatrix, glm::mat4 projectMatrix);
	virtual void DeferredRender(Shader* gShader);
	void ShadowRender(Shader* shadowShader);
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

	RenderingType renderingType = RenderingType::DEFERRED_RENDERING;
private:

	void InitializeShader(const char* vertexShaderPath,
		const char* fragmentShaderPath);
	void InitializeTexture(const char* texturePath);
	void InitializeSquare();
	void InitializeCube();
};

