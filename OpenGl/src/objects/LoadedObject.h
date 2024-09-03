#pragma once
#include "Object.h"
#include "../Model.h"

class LoadedObject : public Object
{
private:
		Material material = Material();
		bool flippedUV;

protected:
		std::string meshName, shaderName;
		Model meshModel;

public:
		LoadedObject() = default;
		LoadedObject(std::string meshName, std::string shaderName, bool flippedUV) : meshName(meshName), shaderName(shaderName), flippedUV(flippedUV)
		{;}

		void SetMaterial(Material mat) { material = mat; }
		
		// Inherited via Object
		void Initialize() override;
		void Render(glm::mat4 view, glm::mat4 projection) override;
};