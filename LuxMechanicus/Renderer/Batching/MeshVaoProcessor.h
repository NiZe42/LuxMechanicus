#pragma once
#include "../../Game/Scenes/Scene.h"

class MeshVaoProcessor
{
public:
	void BuildVaoFromScene(Scene* scene);

	GLuint GetVaoId() const;

	void ClearBuffers();

private:
	GLuint pVaoId, pVboId, pEboId;


};

