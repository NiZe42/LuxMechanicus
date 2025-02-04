#include "Shader.h"

unsigned int Shader::nextShaderId = 0;

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    if (nextShaderId == 0)
        nextShaderId++;

    mShaderProgramId = nextShaderId++;

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

Shader::~Shader() {
    glDeleteProgram(mShaderProgramId);
}

void Shader::Bind() const {
    glUseProgram(mShaderProgramId);
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
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
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
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
}

void Shader::CheckShaderProgramCompilation(int ShaderProgramId) const {
    int success;
    char infoLog[512];

    glGetProgramiv(ShaderProgramId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(mShaderProgramId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

void Shader::SetUniformBool(const std::string& name, bool value) const {
    unsigned int location = glGetUniformLocation(mShaderProgramId, name.c_str());
    glUniform1i(location, (int)value);
}
void Shader::SetUniformInt(const std::string& name, int value) const {
    unsigned int location = glGetUniformLocation(mShaderProgramId, name.c_str());
    glUniform1i(location, value);
}
void Shader::SetUniformFloat(const std::string& name, float value) const {
    unsigned int location = glGetUniformLocation(mShaderProgramId, name.c_str());
    glUniform1f(location, value);
}

void Shader::SetUniformMat4(const std::string& name, glm::mat4 matrix) const {
    unsigned int location = glGetUniformLocation(mShaderProgramId, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

unsigned int Shader::GetShaderProgramId() const {
    return mShaderProgramId;
}