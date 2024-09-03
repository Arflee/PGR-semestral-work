#pragma once
#include "Object.h"

class StaticCubeObject : public Object
{
private:
		unsigned int specularTexture;
		unsigned int diffuseTexture;

public:
		bool CheckCollision() const;
		// Inherited via Object
		void Initialize() override;
		void Render(glm::mat4 view, glm::mat4 projection) override;
};