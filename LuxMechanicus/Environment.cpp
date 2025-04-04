#include "Environment.h"

std::string Environment::rootPath = "";

void Environment::SetRootPath(const char* newRootPath) {
	rootPath = newRootPath;
	std::replace(rootPath.begin(), rootPath.end(), '\\', '/');
}

const char* Environment::GetRootPath() {
	std::cout << "sending path: " << rootPath << std::endl;
	return rootPath.c_str();
}