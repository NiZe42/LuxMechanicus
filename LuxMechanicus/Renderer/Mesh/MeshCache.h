#pragma once
#include <unordered_map>
#include <string>
#include "Mesh.h"

class MeshCache {
public:
    static Mesh* GetMesh(const char* objFilePath);
    static void ClearCache();

private:
    static std::unordered_map<std::string, Mesh*> cache;
};

