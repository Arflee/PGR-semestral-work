#pragma once
#include <pgr.h>

class Transform
{
public:
		glm::vec3 Location;
		float RotationAngle = 0.f;
		glm::vec3 RotationAxis;
		glm::vec3 Scale;

		Transform()
				: Location(0.0f, 0.0f, 0.0f), RotationAxis(glm::vec3(0.0f, 0.0f, 0.0f)), Scale(1.0f, 1.0f, 1.0f)
		{};

		Transform(const glm::vec3& Location, const glm::vec3& Rotation, const glm::vec3& Scale)
				: Location(Location),
				RotationAxis(glm::vec3(glm::radians(Rotation.x), glm::radians(Rotation.y), glm::radians(Rotation.z))),
				Scale(Scale)
		{};
};