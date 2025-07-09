#pragma once	
#include <filesystem>
#include <iostream>

class Environment {
public:
	static std::string rootPath;

	static void SetRootPath(const char* newRootPath);
	static const char* GetRootPath();
};

