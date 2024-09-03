#pragma once
#include <pgr.h>

class Material
{
public:
		Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
				: m_AmbientColor(ambient), m_DiffuseColor(diffuse), m_SpecularColor(specular), m_Shininess(shininess)
		{};
		Material() = default;

		glm::vec3 m_AmbientColor = glm::vec3(0.f);
		glm::vec3 m_DiffuseColor = glm::vec3(0.f);;
		glm::vec3 m_SpecularColor = glm::vec3(0.f);;
		float m_Shininess = 0.f;
};