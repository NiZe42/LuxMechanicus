#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <glad/glad.h> 

struct Vertex {
	glm::vec3 pPosition;
	glm::vec2 pTexCoord;
	glm::vec3 pNormal;

	Vertex(glm::vec3 position,
	glm::vec2 texCoord,
	glm::vec3 normal) {
		pPosition = position;
		pTexCoord = texCoord;
		pNormal = normal;
	}
};

struct IndexVertex {
	int pPositionIndex;
	int pTexCoordIndex;
	int pNormalIndex;

	IndexVertex(int positionIndex,
	int texCoordIndex,
	int normalIndex) {
		pPositionIndex = positionIndex;
		pTexCoordIndex = texCoordIndex;
		pNormalIndex = normalIndex;
	}

	bool operator<(const IndexVertex& other) const {
		if (pPositionIndex != other.pPositionIndex) return pPositionIndex < other.pPositionIndex;
		if (pTexCoordIndex != other.pTexCoordIndex) return pTexCoordIndex < other.pTexCoordIndex;
		return pNormalIndex < other.pNormalIndex;
	}
};

class Mesh
{
public: 
	Mesh(const char* objFilePath);
	~Mesh();

	std::vector<Vertex> GetVertices() const;
	std::vector<unsigned int> GetIndices() const;

	void Render();

private:
	unsigned int pVaoId, pVboId, pEboId;

	std::vector<Vertex> pVertices;
	std::vector<unsigned int> pIndices;
	std::map<IndexVertex, unsigned int> vertexToindexMap;

	void SetupMesh();

	void LoadFromSimpleObjFile(const char* objFilePath);
};

