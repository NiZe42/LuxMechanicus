#include "Mesh.h"
#include "..\..\Profiler\Profiler.h"

unsigned int Mesh::nextMeshId = 0;

Mesh::Mesh(const char* objFilePath) {
	LoadFromSimpleObjFile(objFilePath);

	SetupMesh();

    Id = nextMeshId;
    nextMeshId++;
}

Mesh::~Mesh() {

}

std::vector<Vertex> Mesh::GetVertices() const {
	return pVertices;
}

std::vector<unsigned int> Mesh::GetIndices() const {
	return pIndices;
}

void Mesh::SetupMesh() {
    glGenVertexArrays(1, &pVaoId);
    glGenBuffers(1, &pVboId);
    glGenBuffers(1, &pEboId);

    glBindVertexArray(pVaoId);

    glBindBuffer(GL_ARRAY_BUFFER, pVboId);
    glBufferData(GL_ARRAY_BUFFER, pVertices.size() * sizeof(Vertex), &pVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pEboId);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, pIndices.size() * sizeof(unsigned int), &pIndices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pTexCoord));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pNormal));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::LoadFromSimpleObjFile(const char* objFilePath) {
	std::ifstream file(objFilePath);

    std::cout << "Reading File " << objFilePath << std::endl;

	if (!file.is_open()) {
		std::cerr << "Failed to open OBJ file!\n";
		return;
	}

	std::vector<glm::vec3> tempPositions;
	std::vector<glm::vec2> tempTexCoords;
	std::vector<glm::vec3> tempNormals;

	std::string line;

	while (std::getline(file, line)) {
        std::string prefixString = line.substr(0, line.find_first_of(" "));
        const char* lineString = line.c_str();
        char prefix[10];
        prefix[0] = 0;

        if (prefixString == "v") {
            glm::vec3 position;
            //std::cout << "Parsing: " << lineString << std::endl;
            //std::cout << lineString << std::endl;

            if (sscanf_s(lineString, "%s %f %f %f", prefix, sizeof(prefix), &position.x, &position.y, &position.z) != 4) {
                std::cerr << "Error: Failed to parse line: " << line << std::endl;
                continue;  
            }
            
            tempPositions.push_back(position);
        }
        else if (prefixString == "vt") {
            glm::vec2 texCoord;
            if (sscanf_s(lineString, "%s %f %f", prefix, sizeof(prefix), &texCoord.x, &texCoord.y) != 3) {
                std::cerr << "Error: Failed to parse line: " << line << std::endl;
                continue;  
            }
            tempTexCoords.push_back(texCoord);
        }
        else if (prefixString == "vn") {
            glm::vec3 normal;
            if (sscanf_s(lineString, "%s %f %f %f", prefix, sizeof(prefix), &normal.x, &normal.y, &normal.z) != 4) {
                std::cerr << "Error: Failed to parse line: " << line << std::endl;
                continue;  
            }
            tempNormals.push_back(normal);
        }
        else if (prefixString == "f") {
            
            glm::ivec3 vertexIndex, uvIndex, normalIndex;
            int count = sscanf_s(lineString, "%s %d/%d/%d %d/%d/%d %d/%d/%d",
                prefix, sizeof(prefix), &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                &vertexIndex[1], &uvIndex[1], &normalIndex[1], 
                &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

            if (count != 10)
                return;

            for (int i = 0; i < 3; i++) {
                IndexVertex indexVertex(vertexIndex[i], uvIndex[i], normalIndex[i]);

                std::map<IndexVertex, unsigned int>::iterator found = vertexToindexMap.find(indexVertex);
            
                if (found == vertexToindexMap.end()) {
                    unsigned int index = vertexToindexMap.size();
                    vertexToindexMap[indexVertex] = index;

                    pIndices.push_back(index);

                    Vertex vertex(tempPositions[vertexIndex[i] - 1],
                        tempTexCoords[uvIndex[i] - 1],
                        tempNormals[normalIndex[i] - 1]);

                    pVertices.push_back(vertex);

                } else {
                    unsigned int index = found->second;
                    pIndices.push_back(index);
                }
            }
        }
	}
    file.close();
}

void Mesh::Render() {
    glBindVertexArray(pVaoId);
    glDrawElements(GL_TRIANGLES, pIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
