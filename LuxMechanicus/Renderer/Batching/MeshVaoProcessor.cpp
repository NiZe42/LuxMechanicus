#include "MeshVaoProcessor.h"

void MeshVaoProcessor::BuildVaoFromScene(Scene* scene)
{
    // Clear previous frame's merged mesh
    ClearBuffers();

    std::vector<Vertex> finalVertices;
    std::vector<unsigned int> finalIndices;

    unsigned int vertexOffset = 0;

    for (HierarchyObject* child : scene->GetChildren())
    {
        GameObject* object = dynamic_cast<GameObject*>(child);
        if (!object) continue;

        if (object->GetRenderingType() == RenderingType::FORWARD_RENDERING)
            continue;

        Mesh* mesh = object->GetMesh();
        if (!mesh) continue;

        for (const Vertex& v : mesh->pVertices)
            finalVertices.push_back(v);

        for (unsigned int idx : mesh->pIndices)
            finalIndices.push_back(idx + vertexOffset);

        object->meshMergeIndexOffset = finalIndices.size();
        object->meshMergeIndexCount = mesh->pIndices.size();

        vertexOffset += mesh->pVertices.size();
    }

    if (finalVertices.size() == 0) return;

    glGenVertexArrays(1, &pVaoId);
    glGenBuffers(1, &pVboId);
    glGenBuffers(1, &pEboId);

    glBindVertexArray(pVaoId);

    glBindBuffer(GL_ARRAY_BUFFER, pVboId);
    glBufferData(GL_ARRAY_BUFFER,
        finalVertices.size() * sizeof(Vertex),
        finalVertices.data(),
        GL_DYNAMIC_DRAW);   

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pEboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        finalIndices.size() * sizeof(unsigned int),
        finalIndices.data(),
        GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pTexCoord));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pNormal));

    glBindVertexArray(0);
}

GLuint MeshVaoProcessor::GetVaoId() const {
	return pVaoId;
}

void MeshVaoProcessor::ClearBuffers() {
    if (pVaoId) glDeleteVertexArrays(1, &pVaoId);
    if (pVboId) glDeleteBuffers(1, &pVboId);
    if (pEboId) glDeleteBuffers(1, &pEboId);

    pVaoId = pVboId = pEboId = 0;
}