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

uint64_t Batcher::GenerateSortKey(
    Shader* shader, 
    Texture* texture,
    Mesh* mesh) const
{
    uint64_t key = 0;
    key ^= (uint64_t)shader->GetShaderProgramId();
    key ^= (uint64_t)texture->GetTextureId();
    key ^= ((uint64_t)mesh << 1);
    return key;
}

void Batcher::SortInstances(std::vector<DrawInstance>& list)
{
    std::sort(list.begin(), list.end(),
        [](const DrawInstance& a, const DrawInstance& b)
        {
            return a.sortKey < b.sortKey;
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
        instance.sortKey = GenerateSortKey(
            renderable.shader,
            renderable.texture,
            renderable.mesh);

        mDrawInstances.emplace_back(instance);
    }

    if (mDrawInstances.empty())
        return;

    SortInstances(mDrawInstances);
}

void Batcher::RenderScene(bool isDeferredPass)
{
    glBindVertexArray(mMeshVaoProcessor->GetVaoId());

    uint64_t batchKey = mDrawInstances[0].sortKey;
    uint32_t batchStart = 0u;

    for (uint32_t i = 1; i < mDrawInstances.size(); i++)
    {
        auto& next = mDrawInstances[i];

        if (next.sortKey == batchKey)
            continue;

        auto& instance = mDrawInstances[i - 1];

        if (isDeferredPass) {
            glActiveTexture(GL_TEXTURE0);
            instance.renderable->texture->Bind();
        }

        uint32_t count = i - batchStart;

        CreateInstanceBuffer(&mDrawInstances[batchStart], count);

        Profiler::currentDrawCalls++;
        glDrawElementsInstanced(
            GL_TRIANGLES,
            instance.renderable->object->meshMergeIndexCount,
            GL_UNSIGNED_INT,
            (void*)(sizeof(unsigned int) * instance.renderable->object->meshMergeIndexOffset),
            count
        );

        batchKey = next.sortKey;
        batchStart = i;
    }

    // Last batch
    if (batchStart < mDrawInstances.size())
    {
        auto& instance = mDrawInstances.back();

        if (isDeferredPass) {
            glActiveTexture(GL_TEXTURE0);
            instance.renderable->texture->Bind();
        }

        uint32_t count = mDrawInstances.size() - batchStart;

        CreateInstanceBuffer(&mDrawInstances[batchStart], count);

        Profiler::currentDrawCalls++;
        glDrawElementsInstanced(
            GL_TRIANGLES,
            instance.renderable->object->meshMergeIndexCount,
            GL_UNSIGNED_INT,
            (void*)(sizeof(unsigned int) * instance.renderable->object->meshMergeIndexOffset),
            count
        );
    }

    if (isDeferredPass)
        glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
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

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}