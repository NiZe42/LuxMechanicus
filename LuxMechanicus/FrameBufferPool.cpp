#include "FrameBufferPool.h"
FrameBufferPool::FrameBufferPool() {

}

FrameBufferPool::~FrameBufferPool() {

}

void FrameBufferPool::AddFrameBuffer(FrameBuffer* frameBuffer) {
    pool[frameBuffer->GetFrameBufferType()] = frameBuffer;
}

FrameBuffer* FrameBufferPool::GetFrameBufferOfType(FrameBufferType frameBufferType) const {
    auto element = pool.find(frameBufferType);
    if (element != pool.end()) {
        return element->second;
    }
    return nullptr;
}
