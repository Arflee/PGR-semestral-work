#pragma once
#include "Material.h"
#include <string>
#include <pgr.h>

class Shader
{
public:
		Shader() = default;
		bool LoadShaderFromFile(const std::string& vsPath, const std::string& fsPath);
		void UseShader() const;
		void BindMaterial(const Material& mat) const;
		void SetBoolParameter(const std::string& parameterName, bool value) const;
		void SetIntParameter(const std::string& parameterName, int value) const;
		void SetFloatParameter(const std::string& parameterName, float value) const;
		void SetVec3Parameter(const std::string& parameterName, const glm::vec3& value) const;
		void SetMat4Parameter(const std::string& parameterName, const glm::mat4& value) const;

		GLuint m_ProgramID = -1;
};