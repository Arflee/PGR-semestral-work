#pragma once
#include <pgr.h>
#include "../Transform.h"
#include "../Shader.h"

class Object
{
protected:
		Shader objectShader;
		unsigned int objectVAO;
		unsigned int objectVBO;
		unsigned int objectTexture;
		unsigned int fogTexture;

public:
		Transform transform;
		virtual void Initialize() = 0;
		virtual void Render(glm::mat4 view, glm::mat4 projection) = 0;
		void SetupRender();
		bool isVectorNull(const glm::vec3& vect);
		glm::mat4 AlignObject(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up);
};