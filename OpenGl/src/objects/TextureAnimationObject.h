#pragma once
#include "Object.h"
#include "../Model.h"

class TextureAnimationObject : public Object
{
private:
		float elapsedTime = 0.f;
		float frameDuration = 0.05f;
		float oldTimeSinceStart = 0.f;

		glm::vec2 textureSize = glm::vec2(12, 6);
		float totalDuration = textureSize.x * textureSize.y * frameDuration;
public:
		static bool isSnapped;
		static glm::vec3 torchPosition;
		// Inherited via Object
		void Initialize() override;
		void Render(glm::mat4 view, glm::mat4 projection) override;
		static void SnapTorchToCamera();
};