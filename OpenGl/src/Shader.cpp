#include "Shader.h"
#include <iostream>

bool Shader::LoadShaderFromFile(const std::string& vsPath, const std::string& fsPath)
{
    std::vector<GLuint> shaderList;
    GLuint VS_ID = pgr::createShaderFromFile(GL_VERTEX_SHADER, vsPath);
    if (!VS_ID)
    {
        std::cerr << "Shader::LoadShaderFromFile() Error: Can't create vertex shader with given path: " << vsPath << std::endl;
        return false;
    }
    shaderList.push_back(VS_ID);
    GLuint FS_ID = pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fsPath);
    if (!FS_ID)
    {
        std::cerr << "Shader::LoadShaderFromFile() Error: Can't create fragment shader with given path: " << fsPath << std::endl;
        return false;
    }

    shaderList.push_back(FS_ID);
    GLuint programId = pgr::createProgram(shaderList);
    if (!programId)
    {
        std::cerr << "Shader::LoadShaderFromFile() Error: Can't create shader program from given shaders: " << vsPath << " , " << fsPath << std::endl;
        return false;
    }

    CHECK_GL_ERROR();
    m_ProgramID = programId;
    return true;
}

void Shader::UseShader() const
{
    glUseProgram(m_ProgramID);
}

//void Shader::BindMaterial(const Material& mat) const
//{
//		SetVec3Parameter("material.ambientColor", mat.m_AmbientColor);
//		SetVec3Parameter("material.diffuseColor", mat.m_DiffuseColor);
//		SetVec3Parameter("material.specularColor", mat.m_SpecularColor);
//		SetFloatParameter("material.shininess", mat.m_Shininess);
//}

void Shader::SetBoolParameter(const std::string& parameterName, bool value) const
{
    glUniform1i(glGetUniformLocation(m_ProgramID, parameterName.c_str()), value);
}

void Shader::SetIntParameter(const std::string& parameterName, int value) const
{
    glUniform1i(glGetUniformLocation(m_ProgramID, parameterName.c_str()), value);
}

void Shader::SetFloatParameter(const std::string& parameterName, float value) const
{
    glUniform1f(glGetUniformLocation(m_ProgramID, parameterName.c_str()), value);
}

void Shader::SetVec3Parameter(const std::string& parameterName, const glm::vec3& value) const
{
    GLint uniformLocation = glGetUniformLocation(m_ProgramID, parameterName.c_str());
    if (uniformLocation == -1)
    {
        std::cerr << "Shader::SetVec3Parameter() Error: Can't find parameter with given name: " << parameterName << std::endl;
        return;
    }
    glUniform3fv(uniformLocation, 1, glm::value_ptr(value));
}

void Shader::SetMat4Parameter(const std::string& parameterName, const glm::mat4& value) const
{
    GLint uniformLocation = glGetUniformLocation(m_ProgramID, parameterName.c_str());
    if (uniformLocation == -1)
    {
        std::cerr << "Shader::SetMat4Parameter() Error: Can't find parameter with given name: " << parameterName << std::endl;
        return;
    }
    glUniformMatrix4fv(uniformLocation, 1, false, glm::value_ptr(value));
}
