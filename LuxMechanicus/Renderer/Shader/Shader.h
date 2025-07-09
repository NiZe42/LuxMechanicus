#pragma once
#include "GLIncludes.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


class Shader
{
    // TODO: Implement Builder.
    // That cool builder that would not allow you to create it incomplete.
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    Shader(const char* vertexPath, const char* geometeryPath, const char* fragmentPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;
    
    void SetUniformBool(const std::string& name, bool value) const;
    void SetUniformInt(const std::string& name, int value) const;
    void SetUniformFloat(const std::string& name, float value) const;
    void SetUniformVector3(const std::string& name, glm::vec3 vector) const;
    void SetUniformMat4(const std::string& name, glm::mat4 matrix) const;
    void SetUniformVectorList(const std::string& name, 
        std::vector<glm::vec3> list) const;
    void SetUniformFloatList(const std::string& name,
        std::vector<float> list) const;

    unsigned int GetShaderProgramId() const;

private:
    unsigned int mShaderProgramId;
    static unsigned int nextShaderId;

    std::string LoadShaderSource(GLenum shaderType, const char* shaderPath);

    void CheckShaderCompilation(int shaderId) const;
    void CheckShaderProgramCompilation(int shaderProgramId) const;
};
