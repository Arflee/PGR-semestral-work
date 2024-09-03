#include "Object.h"
#include "TextureAnimationObject.h"
#include "../Camera.h"

void Object::SetupRender()
{
		objectShader.UseShader();

		objectShader.SetVec3Parameter("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		objectShader.SetVec3Parameter("dirLight.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		objectShader.SetVec3Parameter("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		objectShader.SetVec3Parameter("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		// point light 1
		objectShader.SetVec3Parameter("pointLight.position", TextureAnimationObject::torchPosition);
		objectShader.SetVec3Parameter("pointLight.ambient", glm::vec3(0.2f, 0.2f, 0.20f));
		objectShader.SetVec3Parameter("pointLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		objectShader.SetVec3Parameter("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		objectShader.SetFloatParameter("pointLight.constant", 1.0f);
		objectShader.SetFloatParameter("pointLight.linear", 0.09f);
		objectShader.SetFloatParameter("pointLight.quadratic", 0.032f);

		// spotLight
		objectShader.SetVec3Parameter("spotLight.position", Camera::MainCamera.Position);
		objectShader.SetVec3Parameter("spotLight.direction", Camera::MainCamera.Front);
		objectShader.SetVec3Parameter("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		objectShader.SetVec3Parameter("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		objectShader.SetVec3Parameter("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		objectShader.SetFloatParameter("spotLight.constant", 1.0f);
		objectShader.SetFloatParameter("spotLight.linear", 0.09f);
		objectShader.SetFloatParameter("spotLight.quadratic", 0.032f);
		objectShader.SetFloatParameter("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		objectShader.SetFloatParameter("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		objectShader.SetFloatParameter("material.shininess", 32.0f);
}

bool Object::isVectorNull(const glm::vec3& vect) {

		return !vect.x && !vect.y && !vect.z;
}

glm::mat4 Object::AlignObject(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up) {

		glm::vec3 z = -glm::normalize(front);

		if (isVectorNull(z))
				z = glm::vec3(0.0, 0.0, 1.0);

		glm::vec3 x = glm::normalize(glm::cross(up, z));

		if (isVectorNull(x))
				x = glm::vec3(1.0, 0.0, 0.0);

		glm::vec3 y = glm::cross(z, x);
		//mat4 matrix = mat4(1.0f);
		glm::mat4 matrix = glm::mat4(
				x.x, x.y, x.z, 0.0,
				y.x, y.y, y.z, 0.0,
				z.x, z.y, z.z, 0.0,
				position.x, position.y, position.z, 1.0
		);

		return matrix;
}