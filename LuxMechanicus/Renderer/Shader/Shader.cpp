#include "Shader.h"

unsigned int Shader::nextShaderId = 0;

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    /*if (nextShaderId == 0)
        nextShaderId++;

    mShaderProgramId = nextShaderId++;*/

    std::cout << "Compiling vertex shader : " << vertexPath << std::endl;
    std::cout << "Compiling fragment shader : " << fragmentPath << std::endl;

    std::string vertexShaderCode = LoadShaderSource(GL_VERTEX_SHADER, vertexPath);
    std::string fragmentShaderCode = LoadShaderSource(GL_FRAGMENT_SHADER, fragmentPath);

    const char* vertexShaderCodePointer = vertexShaderCode.c_str();
    const char* fragmentShaderCodePointer = fragmentShaderCode.c_str();

    unsigned int vertexShaderId;
    
    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexShaderCodePointer, NULL);
    glCompileShader(vertexShaderId);

    CheckShaderCompilation(vertexShaderId);
    
    unsigned int fragmentShaderId;

    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragmentShaderCodePointer, NULL);
    glCompileShader(fragmentShaderId);
     
    CheckShaderCompilation(fragmentShaderId);

    mShaderProgramId = glCreateProgram();
    glAttachShader(mShaderProgramId, vertexShaderId);
    glAttachShader(mShaderProgramId, fragmentShaderId);
    glLinkProgram(mShaderProgramId);
    
    CheckShaderProgramCompilation(mShaderProgramId);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

Shader::Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{
    std::cout << "Compiling vertex shader : " << vertexPath << std::endl;
    std::string vertexShaderCode = LoadShaderSource(GL_VERTEX_SHADER, vertexPath);
    const char* vertexShaderCodePointer = vertexShaderCode.c_str();

    unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexShaderCodePointer, NULL);
    glCompileShader(vertexShaderId);
    CheckShaderCompilation(vertexShaderId);

    std::cout << "Compiling fragment shader : " << fragmentPath << std::endl;
    std::string fragmentShaderCode = LoadShaderSource(GL_FRAGMENT_SHADER, fragmentPath);
    const char* fragmentShaderCodePointer = fragmentShaderCode.c_str();

    unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragmentShaderCodePointer, NULL);
    glCompileShader(fragmentShaderId);
    CheckShaderCompilation(fragmentShaderId);

    unsigned int geometryShaderId = 0;
    bool hasGeometryShader = geometryPath != nullptr && geometryPath[0] != '\0';
    if (hasGeometryShader) {
        std::cout << "Compiling geometry shader : " << geometryPath << std::endl;
        std::string geometryShaderCode = LoadShaderSource(GL_GEOMETRY_SHADER, geometryPath);
        const char* geometryShaderCodePointer = geometryShaderCode.c_str();

        geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShaderId, 1, &geometryShaderCodePointer, NULL);
        glCompileShader(geometryShaderId);
        CheckShaderCompilation(geometryShaderId);
    }

    mShaderProgramId = glCreateProgram();
    glAttachShader(mShaderProgramId, vertexShaderId);
    if (hasGeometryShader) {
        glAttachShader(mShaderProgramId, geometryShaderId);
    }
    glAttachShader(mShaderProgramId, fragmentShaderId);

    glLinkProgram(mShaderProgramId);
    CheckShaderProgramCompilation(mShaderProgramId);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    if (hasGeometryShader) {
        glDeleteShader(geometryShaderId);
    }
}


Shader::~Shader() {
    glDeleteProgram(mShaderProgramId);
}

void Shader::Bind() const {
    glUseProgram(mShaderProgramId);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

std::string Shader::LoadShaderSource(GLenum shaderType, const char* shaderPath) {
    std::string shaderSource;
    std::ifstream shaderFile;
    
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shaderFile.open(shaderPath);
        std::stringstream shaderStream;

        shaderStream << shaderFile.rdbuf();
        
        shaderFile.close();
        
        shaderSource = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "Couldn't read shader file" << std::endl;
    }
    return shaderSource;
}

void Shader::CheckShaderCompilation(int shaderId) const{
    int success;
    char infoLog[512];

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        std::cout << "Couldn't compile shader file\n" << infoLog << std::endl;
    };
}

void Shader::CheckShaderProgramCompilation(int ShaderProgramId) const {
    int success;
    char infoLog[512];

    glGetProgramiv(ShaderProgramId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(mShaderProgramId, 512, NULL, infoLog);
        std::cout << "Couldn't link shader\n" << infoLog << std::endl;
    }
}

void Shader::SetUniformBool(const std::string& name, bool value) const {
    unsigned int location = glGetUniformLocation(mShaderProgramId, name.c_str());
    if (location == -1) {
        std::cout << "No such variable in the shader: " << name << std::endl;
        return;
    }
    glUniform1i(location, (int)value);
}
void Shader::SetUniformInt(const std::string& name, int value) const {
    unsigned int location = glGetUniformLocation(mShaderProgramId, name.c_str());
    if (location == -1) {
        std::cout << "No such variable in the shader: " << name << std::endl;
        return;
    }
    glUniform1i(location, value);
}
void Shader::SetUniformFloat(const std::string& name, float value) const {
    unsigned int location = glGetUniformLocation(mShaderProgramId, name.c_str());
    if (location == -1) {
        std::cout << "No such variable in the shader: " << name << std::endl;
        return;
    }
    glUniform1f(location, value);
}
void Shader::SetUniformMat4(const std::string& name, glm::mat4 matrix) const {
    unsigned int location = glGetUniformLocation(mShaderProgramId, name.c_str());
    if (location == -1) {
        std::cout << "No such variable in the shader: " << name << std::endl;
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::SetUniformVectorList(const std::string& name, std::vector<glm::vec3> list) const{
    if (list.empty()) return;
    unsigned int location = glGetUniformLocation(mShaderProgramId, name.c_str());
    if (location == -1) {
        std::cout << "No such variable in the shader: " << name << std::endl;
        return;
    }
    glUniform3fv(location, list.size(), glm::value_ptr(list[0]));
}
void Shader::SetUniformFloatList(const std::string& name, std::vector<float> list) const {
    if (list.empty()) return;
    unsigned int location = glGetUniformLocation(mShaderProgramId, name.c_str());
    if (location == -1) {
        std::cout << "No such variable in the shader: " << name << std::endl;
        return;
    }
    glUniform1fv(location, list.size(), list.data());
}
void Shader::SetUniformVector3(const std::string& name, glm::vec3 vector) const {
    unsigned int location = glGetUniformLocation(mShaderProgramId, name.c_str());
    if (location == -1) {
        std::cout << "No such variable in the shader: " << name << std::endl;
        return;
    }
    glUniform3fv(location, 1, glm::value_ptr(vector));
}


unsigned int Shader::GetShaderProgramId() const {
    return mShaderProgramId;
}