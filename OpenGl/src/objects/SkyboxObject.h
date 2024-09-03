#pragma once
#include "Object.h"
#include "../stb_image.h"
#include "../Model.h"
#include "../Camera.h"

class SkyboxObject : public Object
{
public:
		void Initialize() override;
		void Render(glm::mat4 view, glm::mat4 projection) override;
};