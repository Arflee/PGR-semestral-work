#include "EagleObject.h"
#include "../Camera.h"
#include "TextureAnimationObject.h"
#include <iostream>

float rotationAngle = 0.f;

Transform EagleObject::eaglePosition;

void EagleObject::Initialize()
{
		LoadedObject::Initialize();
}

void EagleObject::Render(glm::mat4 view, glm::mat4 projection)
{
		transform.RotationAngle = rotationAngle;
		eaglePosition = transform;
		if (rotationAngle > 360.f) rotationAngle -= 360.f;

		LoadedObject::SetupRender();

		// view/projection transformations
		objectShader.SetMat4Parameter("projection", projection);
		objectShader.SetMat4Parameter("view", view);

		float radius = 3.f;

		transform.Location = glm::vec3(
				radius * glm::cos(glm::radians(rotationAngle)),
				3.5f,
				radius * glm::sin(glm::radians(rotationAngle))
		);

		glm::vec3 tangent = -glm::normalize(glm::vec3(-transform.Location.z, 0.f, transform.Location.x));
		glm::vec3 objectUp = glm::vec3(0.f, 1.f, 0.f);
		glm::vec3 rotationAxis = glm::cross(objectUp, tangent);
		float tangentRotation = glm::acos(glm::dot(objectUp, tangent));

		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, transform.Location);
		model = AlignObject(transform.Location, tangent, objectUp);
		model = glm::scale(model, transform.Scale);
		model = glm::rotate(model, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
		model = glm::rotate(model, glm::radians(tangentRotation), rotationAxis);
		objectShader.SetMat4Parameter("model", model);
		meshModel.Draw(objectShader);
		rotationAngle += 0.05f;
}
