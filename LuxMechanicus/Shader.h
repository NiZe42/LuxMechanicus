#pragma once
#include <glad/glad.h> 
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void Bind() const;
    
    void SetUniformBool(const std::string& name, bool value) const;
    void SetUniformInt(const std::string& name, int value) const;
    void SetUniformFloat(const std::string& name, float value) const;
    void SetUniformMat4(const std::string& name, glm::mat4 matrix) const;

    unsigned int GetShaderProgramId() const;

private:
    unsigned int mShaderProgramId;
    static unsigned int nextShaderId;

    std::string LoadShaderSource(GLenum shaderType, const char* shaderPath);

    void CheckShaderCompilation(int shaderId) const;
    void CheckShaderProgramCompilation(int shaderProgramId) const;
};
