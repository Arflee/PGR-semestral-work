#pragma once
#include "Object.h"

class MovingTextureObject : public Object
{
public:
		static bool isDone;
		// Inherited via Object
		void Initialize() override;
		void Render(glm::mat4 view, glm::mat4 projection) override;
};