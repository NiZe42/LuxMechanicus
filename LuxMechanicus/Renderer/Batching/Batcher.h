#pragma once
#include <vector>
#include <cstdint>
#include "../Mesh/Mesh.h"
#include "../Shader/Shader.h"
#include "../../Game/Scenes/Scene.h"
#include "../Batching/MeshVaoProcessor.h"
#include "..\..\Utils\GLIncludes.h"

struct BatcherRenderable
{
    GameObject* object = nullptr;
    Shader* shader = nullptr;
    Texture* texture = nullptr;
    Mesh* mesh = nullptr;
};

struct DrawInstance
{
    BatcherRenderable* renderable;
    uint64_t sortKey = 0;
};

class Batcher
{
public:
    Batcher();
    ~Batcher();

    void BatchScene(Scene* scene);
    void RenderScene(bool isDeferredPass);

private:

    uint64_t GenerateSortKey(Shader* shader, Texture* texture,  Mesh* mesh) const;
    void SortInstances(std::vector<DrawInstance>& list);

private:
    MeshVaoProcessor* mMeshVaoProcessor;

    GLuint mInstanceVbo = 0;

    std::vector<BatcherRenderable> mRenderables;
    std::vector<DrawInstance> mDrawInstances;

    void CreateInstanceBuffer(const DrawInstance* instances, uint32_t count);
};
