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
    uint64_t materialKey = 0;
    uint64_t meshKey = 0;
};

struct DrawElementsIndirectCommand {
    GLuint count;       
    GLuint instanceCount;
    GLuint firstIndex; 
    GLuint baseVertex;  
    GLuint baseInstance;
};

class Batcher
{
public:
    Batcher();
    ~Batcher();

    void BatchScene(Scene* scene);
    void RenderScene(bool isDeferredPass);

private:

    uint64_t GenerateMaterialKey(
        Shader* shader,
        Texture* texture) const;
    uint64_t GenerateMeshKey(Mesh* mesh) const;
    void SortInstancesByMaterial(std::vector<DrawInstance>& list);

    void CreateDrawIndirectCommands(DrawInstance* instances,
        uint32_t count,
        uint32_t batchStart,
        std::vector<DrawElementsIndirectCommand>& outCommands);

    MeshVaoProcessor* mMeshVaoProcessor;

    GLuint mInstanceVbo = 0;
    GLuint mIndirectBuffer = 0;

    std::vector<BatcherRenderable> mRenderables;
    std::vector<DrawInstance> mDrawInstances;

    void CreateInstanceBuffer(const DrawInstance* instances, uint32_t count);
};
