#pragma once
#include "LoadedObject.h"

class TransparentObject : public LoadedObject
{

public:
		// Inherited via Object
		void Initialize() override;
		void Render(glm::mat4 view, glm::mat4 projection) override;
};