#pragma once
#include "LoadedObject.h"

class EagleObject : public LoadedObject
{
public:
		static Transform eaglePosition;

		EagleObject() = default;
		EagleObject(std::string meshName, std::string shaderName) : LoadedObject(meshName, shaderName, false)
		{
		}
		void Initialize() override;
		void Render(glm::mat4 view, glm::mat4 projection) override;
};