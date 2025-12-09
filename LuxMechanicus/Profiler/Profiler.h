#pragma once
#include "..\Utils\GLIncludes.h"
#include <vector>
#include <string>
#include <queue>
#include <chrono>

struct FrameRecord
{
    double cpuFrameMs = 0.0;
    double gpuFrameMs = 0.0;
    size_t vramBytes = 0;
    int drawCalls = 0;
};

class Profiler
{
public:
    Profiler();
    ~Profiler();

    void BeginFrame();              
    void EndFrame();

    void WriteAllToCSV(const std::string& filePath);

    static int currentDrawCalls;

private:

    struct GPUFrameQueries
    {
        GLuint startQuery;
        GLuint endQuery;
        int frameIndex;
    };

    std::chrono::high_resolution_clock::time_point cpuFrameStart;

    std::vector<FrameRecord> records;
    std::queue<GPUFrameQueries> pendingQueries;

    FrameRecord currentFrame;

    void BeginGPUQuery();
    void EndGPUQuery();

    void TryResolveGPUQueries();

    size_t GetVRAMUsageBytes();
};
