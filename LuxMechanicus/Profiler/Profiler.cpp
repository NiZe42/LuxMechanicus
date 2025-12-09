#include "Profiler.h"
#include <fstream>
#include <iostream>

int Profiler::currentDrawCalls = 0;

Profiler::Profiler()
{
}

Profiler::~Profiler()
{
    while (!pendingQueries.empty())
    {
        auto& q = pendingQueries.front();
        glDeleteQueries(1, &q.startQuery);
        glDeleteQueries(1, &q.endQuery);
        pendingQueries.pop();
    }
}

void Profiler::BeginFrame()
{
    cpuFrameStart = std::chrono::high_resolution_clock::now();

    currentFrame = FrameRecord();
    currentDrawCalls = 0;

    BeginGPUQuery();
}

void Profiler::EndFrame()
{
    auto now = std::chrono::high_resolution_clock::now();
    currentFrame.cpuFrameMs =
        std::chrono::duration<double, std::milli>(now - cpuFrameStart).count();

    currentFrame.drawCalls = currentDrawCalls;

    currentFrame.vramBytes = GetVRAMUsageBytes();

    EndGPUQuery();

    records.push_back(currentFrame);

    TryResolveGPUQueries();
}

void Profiler::BeginGPUQuery()
{
    GLuint qStart, qEnd;
    glGenQueries(1, &qStart);
    glGenQueries(1, &qEnd);

    glQueryCounter(qStart, GL_TIMESTAMP);

    GPUFrameQueries queries;
    queries.startQuery = qStart;
    queries.endQuery = qEnd;
    queries.frameIndex = (int)records.size();

    pendingQueries.push(queries);
}

void Profiler::EndGPUQuery()
{ 
    glQueryCounter(pendingQueries.back().endQuery, GL_TIMESTAMP);    
}

void Profiler::TryResolveGPUQueries()
{
    while (!pendingQueries.empty())
    {
        auto& pair = pendingQueries.front();

        GLuint available = 0;
        glGetQueryObjectuiv(pair.endQuery, GL_QUERY_RESULT_AVAILABLE, &available);

        if (!available)
            return;

        GLuint64 startTimeStamp = 0;
        GLuint64 endTimeStamp = 0;

        glGetQueryObjectui64v(pair.startQuery, GL_QUERY_RESULT, &startTimeStamp);
        glGetQueryObjectui64v(pair.endQuery, GL_QUERY_RESULT, &endTimeStamp);

        double gpuMs = double(endTimeStamp - startTimeStamp) / 1e6;

        if (pair.frameIndex < records.size())
            records[pair.frameIndex].gpuFrameMs = gpuMs;

        glDeleteQueries(1, &pair.startQuery);
        glDeleteQueries(1, &pair.endQuery);

        pendingQueries.pop();
    }
}

size_t Profiler::GetVRAMUsageBytes()
{
    GLint totalMemoryKb = 0;
    glGetIntegerv(0x9048, &totalMemoryKb); // GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX  
    GLint currentMemoryKb = 0;
    glGetIntegerv(0x9049, &currentMemoryKb); // GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX

    if (totalMemoryKb == 0)
        return 0;

    return size_t(totalMemoryKb - currentMemoryKb) * 1024;
}

void Profiler::WriteAllToCSV(const std::string& filePath)
{
    std::ofstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open profiler CSV file\n";
        return;
    }

    file << "Frame,CPU(ms),GPU(ms),VRAM(bytes),DrawCalls\n";

    for (size_t i = 0; i < records.size(); i++)
    {
        const auto& r = records[i];
        file << i << ","
            << r.cpuFrameMs << ","
            << r.gpuFrameMs << ","
            << r.vramBytes << ","
            << r.drawCalls << "\n";
    }

    file.close();
    std::cout << "Profiler CSV saved: " << filePath << "\n";
}
