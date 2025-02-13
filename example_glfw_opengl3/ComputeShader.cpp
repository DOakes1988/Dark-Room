#include <glad/glad.h>
#include "ComputeShader.h"
#include <iostream>
#include <fstream>
#include <sstream>


ComputeShader::ComputeShader(const std::string& shaderPath)
{
    unsigned int computeShader = loadComputeShader(shaderPath);
    if (computeShader == 0) {
        std::cout << "ERROR::SHADER::LOAD_FAILED: " << shaderPath << std::endl;
        return;
    }

    ID = glCreateProgram();

    if (ID == 0) {
        std::cout << "ERROR::PROGRAM_CREATION_FAILED" << std::endl;
        return;
    }

    glAttachShader(ID, computeShader);
    glLinkProgram(ID);
    checkLinkErrors(ID);
    glDeleteShader(computeShader);
}

ComputeShader::~ComputeShader()
{
    std::cout << "PROGRAM: " << ID << std::endl;
    if (ID != 0)
    {
        glDeleteProgram(ID);
    }
    std::cout << "DELETED: " << ID << std::endl;
}

void ComputeShader::use()
{
    if (ID != 0)
    {
        glUseProgram(ID);
    }
}

void ComputeShader::setUniform(const std::string& name, float value)
{
    if (ID != 0) {
        GLint location = glGetUniformLocation(ID, name.c_str());
        if (location != -1) {
            glUniform1f(location, value);
        }
        else {
            std::cerr << "WARNING::UNIFORM::" << name << " not found in shader program!" << std::endl;
        }
    }
    else {
        std::cerr << "ERROR::PROGRAM::SET_UNIFORM_FAILED: Program not initialized." << std::endl;
    }
}

unsigned int ComputeShader::loadComputeShader(const std::string& shaderPath)
{
    std::ifstream shaderFile(shaderPath);
    shaderFile.open(shaderPath);

    if (!shaderFile.is_open()) {
        std::cerr << "ERROR::SHADER::FILE_NOT_READABLE: " << shaderPath << std::endl;
        return 0;
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();

    shaderFile.close();

    std::string shaderCode = shaderStream.str();
    const char* shaderSource = shaderCode.c_str();

    unsigned int shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);
    checkCompileErrors(shader);

    return shader;
}

void ComputeShader::bindTexture(unsigned int texture, unsigned int binding)
{
    glBindImageTexture(binding, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
}

int ComputeShader::genBuffer()
{
    unsigned int buf;
    glGenBuffers(1, &buf);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf);
    return buf;
}

void ComputeShader::bindBuffer(unsigned int buf, unsigned int index)
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, buf);
}

void ComputeShader::setBuffer(unsigned int buf, size_t size, void* data)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_READ);
}

void ComputeShader::set1fv(const std::string& name, int num, float value[]) const
{
    glUniform1fv(glGetUniformLocation(ID, name.c_str()), num, value);
}

void ComputeShader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void ComputeShader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ComputeShader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ComputeShader::setVec2(const std::string& name, const ImVec2& value) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
}

void ComputeShader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void ComputeShader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void ComputeShader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void ComputeShader::setMat3(const std::string& name, const Matrix3x3& m) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &m[0][0]);
}

void ComputeShader::checkCompileErrors(unsigned int shader)
{
    int success;
	char infoLog[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cerr << "Compute Shader Compile Error: " << "\n" << infoLog << std::endl;
	}
}

void ComputeShader::checkLinkErrors(unsigned int program)
{
    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}
