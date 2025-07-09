#include "MeshCache.h"
#include <iostream>


std::unordered_map<std::string, Mesh*> MeshCache::cache;

Mesh* MeshCache::GetMesh(const char* objFilePath) { 
    
    auto it = cache.find(std::string(objFilePath));
    if (it != cache.end()) {
        return it->second; 
    }
    Mesh* newMesh = new Mesh(objFilePath);
    cache[std::string(objFilePath)] = newMesh;

    return newMesh;
}

void MeshCache::ClearCache() {
    for (auto& pair : cache) {
        delete pair.second; 
    }
    cache.clear();
}
