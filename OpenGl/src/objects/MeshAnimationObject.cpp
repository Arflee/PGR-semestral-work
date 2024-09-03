#include "MeshAnimationObject.h"
#include <iostream>

void MeshAnimationObject::Initialize()
{
		LoadedObject::Initialize();
}

float meshRotation = 0.f;

float MeshAnimationObject::rotationSpeed = 0.055f;

void MeshAnimationObject::Render(glm::mat4 view, glm::mat4 projection)
{
    if (meshRotation >= 360.f)
    {
        meshRotation -= 360.f;
    }

    LoadedObject::SetupRender();

		objectShader.SetMat4Parameter("projection", projection);
		objectShader.SetMat4Parameter("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, transform.Location);
		model = glm::scale(model, transform.Scale);
		model = glm::rotate(model, glm::radians(transform.RotationAngle), transform.RotationAxis);
		model = glm::rotate(model, glm::radians(meshRotation), glm::vec3(1.f, 0.f, 0.f));
		//model = AlignObject(transform.Location, tangent, glm::vec3(0.f, 1.f, 0.f));
		objectShader.SetMat4Parameter("model", model);
		meshModel.Draw(objectShader);

		glBindVertexArray(0);
		glUseProgram(0);
    meshRotation += rotationSpeed;
}
