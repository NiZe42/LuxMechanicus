#include "FrameBufferPool.h"
FrameBufferPool::FrameBufferPool() {

}

FrameBufferPool::~FrameBufferPool() {
    for (auto& pair : pool) {
        delete pair.second; 
    }

    pool.clear();
}

void FrameBufferPool::AddFrameBuffer(FrameBuffer* frameBuffer) {
    pool[frameBuffer->GetFrameBufferType()] = frameBuffer;
}


FrameBuffer* FrameBufferPool::GetFrameBufferOfType(FrameBufferType frameBufferType) {
    if (frameBufferType == FrameBufferType::None) {
        std::cerr << "Post-Processing Effect is not initialized" << std::endl;
        return nullptr;
    }

    auto element = pool.find(frameBufferType);
    if (element != pool.end()) {
        currentFrameBuffer = element->second;
        return element->second;
    }

    std::cerr << "No FrameBuffer in the pool with requested type" << std::endl;
    return nullptr;
}

void FrameBufferPool::Initialize() {

}

FrameBuffer* FrameBufferPool::GetCurrentlySelectedFrameBuffer() const {
    return currentFrameBuffer;
}
