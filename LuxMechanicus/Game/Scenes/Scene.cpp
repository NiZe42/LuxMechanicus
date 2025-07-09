#include "Scene.h"

unsigned int Scene::nextSceneId = 0;

Scene::Scene() {
	if (nextSceneId == 0) 
		nextSceneId++;
	
	mSceneId = nextSceneId++;
}

Scene::~Scene() {
	
}

void Scene::Render() {
	
}

void Scene::RenderTriangle() {
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};

	unsigned int vaoId;
	glGenVertexArrays(1, &vaoId);

	glBindVertexArray(vaoId);

	unsigned int vboId;
	glGenBuffers(1, &vboId);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

unsigned int Scene::GetSceneId() const {
	return mSceneId;
}