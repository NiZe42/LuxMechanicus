#include "Batcher.h"
#include <algorithm>
#include <span>
#include "..\..\Profiler\Profiler.h"

Batcher::Batcher()
{
    mMeshVaoProcessor = new MeshVaoProcessor();
}

Batcher::~Batcher()
{
    if (mMeshVaoProcessor)
        delete mMeshVaoProcessor;
}

uint64_t Batcher::GenerateMaterialKey(
    Shader* shader, 
    Texture* texture) const
{
    uint64_t key = 0;
    key ^= (uint64_t)shader->engineShaderId;
    key ^= (uint64_t)texture->engineTextureId;
    return key;
}

uint64_t Batcher::GenerateMeshKey(Mesh* mesh) const
{
    uint64_t key = 0;
    key ^= ((uint64_t)mesh << 1);
    return key;
}


void Batcher::SortInstancesByMaterial(std::vector<DrawInstance>& list)
{
    std::sort(list.begin(), list.end(),
        [](const DrawInstance& a, const DrawInstance& b)
        {
            return a.materialKey < b.materialKey;
        });
}

void Batcher::BatchScene(Scene* scene)
{
    mRenderables.clear();
    mDrawInstances.clear();

    for (HierarchyObject* child : scene->GetChildren())
    {
        GameObject* obj = dynamic_cast<GameObject*>(child);
        if (!obj) continue;

        if (obj->GetRenderingType() == RenderingType::FORWARD_RENDERING)
            continue;

        Mesh* mesh = obj->GetMesh();
        Shader* shader = obj->GetShader();
        Texture* texture = obj->GetTexture();

        if (!mesh || !shader || !texture) continue;
        mRenderables.push_back({ obj, shader, texture, mesh });
    }

    mMeshVaoProcessor->BuildVaoFromScene(scene);
    
    mDrawInstances.clear();

    for (auto& renderable : mRenderables)
    {
        DrawInstance instance{};
        instance.renderable = &renderable;
        instance.materialKey = GenerateMaterialKey(
            renderable.shader,
            renderable.texture);
        instance.meshKey = GenerateMeshKey(renderable.mesh);
        mDrawInstances.emplace_back(instance);
    }

    if (mDrawInstances.empty())
        return;

    SortInstancesByMaterial(mDrawInstances);
    std::cout << mDrawInstances[0].materialKey << " ";
    std::cout << mDrawInstances[1].materialKey << " ";
    std::cout << mDrawInstances[2].materialKey << " ";
    std::cout << mDrawInstances[3].materialKey << " ";
}

void Batcher::RenderScene(bool isDeferredPass)
{
    glBindVertexArray(mMeshVaoProcessor->GetVaoId());

    uint64_t batchKey = mDrawInstances[0].materialKey;
    uint32_t batchStart = 0u;

    for (uint32_t i = 1; i < mDrawInstances.size(); i++)
    {
        auto& next = mDrawInstances[i];

        if (next.materialKey == batchKey)
            continue;

        std::sort(mDrawInstances.begin() + batchStart, mDrawInstances.begin() + i,
            [](const DrawInstance& a, const DrawInstance& b) { return a.meshKey < b.meshKey; });

        auto& instance = mDrawInstances[i - 1];

        if (isDeferredPass) {
            glActiveTexture(GL_TEXTURE0);
            instance.renderable->texture->Bind();
        }

        uint32_t count = i - batchStart;

        CreateInstanceBuffer(&mDrawInstances[batchStart], count);

        std::vector<DrawElementsIndirectCommand> indirectCommands;
        CreateDrawIndirectCommands(&mDrawInstances[batchStart], count, batchStart, indirectCommands);

        if (mIndirectBuffer == 0)
            glGenBuffers(1, &mIndirectBuffer);

        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, mIndirectBuffer);
        glBufferData(GL_DRAW_INDIRECT_BUFFER,
            indirectCommands.size() * sizeof(DrawElementsIndirectCommand),
            indirectCommands.data(),
            GL_DYNAMIC_DRAW);

        Profiler::currentDrawCalls++;

        glMultiDrawElementsIndirect(
            GL_TRIANGLES,
            GL_UNSIGNED_INT,
            (void*)0,
            indirectCommands.size(),
            0);

        batchKey = next.materialKey;
        batchStart = i;
    }

    // Last batch
    if (batchStart < mDrawInstances.size())
    {
        std::sort(mDrawInstances.begin() + batchStart, mDrawInstances.end(),
            [](const DrawInstance& a, const DrawInstance& b) { return a.meshKey < b.meshKey; });

        auto& instance = mDrawInstances.back();

        if (isDeferredPass) {
            glActiveTexture(GL_TEXTURE0);
            instance.renderable->texture->Bind();
        }

        uint32_t count = mDrawInstances.size() - batchStart;

        CreateInstanceBuffer(&mDrawInstances[batchStart], count);

        std::vector<DrawElementsIndirectCommand> indirectCommands;
        CreateDrawIndirectCommands(&mDrawInstances[batchStart], count, batchStart, indirectCommands);

        if (mIndirectBuffer == 0) 
            glGenBuffers(1, &mIndirectBuffer);

        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, mIndirectBuffer);
        glBufferData(GL_DRAW_INDIRECT_BUFFER,
            indirectCommands.size() * sizeof(DrawElementsIndirectCommand),
            indirectCommands.data(),
            GL_DYNAMIC_DRAW);

        Profiler::currentDrawCalls++;
        glMultiDrawElementsIndirect(
            GL_TRIANGLES,
            GL_UNSIGNED_INT,
            (void*)0,
            indirectCommands.size(),
            0);
    }

    if (isDeferredPass)
        glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
}

void Batcher::CreateInstanceBuffer(const DrawInstance* instances, uint32_t count)
{
    if (!instances || count == 0)
        return;

    std::vector<glm::mat4> modelMatrices;

    for (uint32_t i = 0; i < count; ++i)
    {
        GameObject* object = instances[i].renderable->object;
        modelMatrices.push_back(object->GetModelMatrix());
    }

    if (mInstanceVbo == 0)
        glGenBuffers(1, &mInstanceVbo);

    glBindBuffer(GL_ARRAY_BUFFER, mInstanceVbo);
    glBufferData(GL_ARRAY_BUFFER,
        modelMatrices.size() * sizeof(glm::mat4),
        modelMatrices.data(),
        GL_DYNAMIC_DRAW);

    size_t vec4Size = sizeof(glm::vec4);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(vec4Size * 1));
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(vec4Size * 2));
    glVertexAttribDivisor(5, 1);

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(vec4Size * 3));
    glVertexAttribDivisor(6, 1);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Batcher::CreateDrawIndirectCommands(
    DrawInstance* instances,
    uint32_t count,
    uint32_t batchStart,
    std::vector<DrawElementsIndirectCommand>& outCommands)
{
    if (!instances || count == 0)
        return;

    outCommands.clear();

    uint32_t meshBatchStart = 0;

    for (uint32_t i = 1; i < count; ++i)
    {
        if (instances[i].meshKey != instances[meshBatchStart].meshKey)
        {
            auto& firstInstance = instances[meshBatchStart];
            DrawElementsIndirectCommand cmd{};
            cmd.count = firstInstance.renderable->object->meshMergeIndexCount;
            cmd.instanceCount = i - meshBatchStart;
            cmd.firstIndex = firstInstance.renderable->object->meshMergeIndexOffset;
            cmd.baseVertex = 0;
            cmd.baseInstance = meshBatchStart;

            outCommands.push_back(cmd);

            meshBatchStart = i;
        }
    }

    if (meshBatchStart < count)
    {
        auto& firstInstance = instances[meshBatchStart];

        DrawElementsIndirectCommand cmd{};
        cmd.count = firstInstance.renderable->object->meshMergeIndexCount;
        cmd.instanceCount = count - meshBatchStart;
        cmd.firstIndex = firstInstance.renderable->object->meshMergeIndexOffset;
        cmd.baseVertex = 0;
        cmd.baseInstance = meshBatchStart;

        outCommands.push_back(cmd);
    }
}