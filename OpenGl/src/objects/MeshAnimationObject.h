#pragma once
#include "LoadedObject.h"

class MeshAnimationObject : public LoadedObject
{
public:
		MeshAnimationObject(std::string meshName, std::string shaderName, bool flippedUV) :
				LoadedObject(meshName, shaderName, flippedUV)
		{}

		// Inherited via Object
		void Initialize() override;
		void Render(glm::mat4 view, glm::mat4 projection) override;

		static float rotationSpeed;
};