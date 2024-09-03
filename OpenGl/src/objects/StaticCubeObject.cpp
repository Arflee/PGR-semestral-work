#include "StaticCubeObject.h"
#include "../Model.h"
#include "../Camera.h"
#include "../Input.h"
#include "TextureAnimationObject.h"
#include <iostream>

float colliderRadius = 1.0;

bool StaticCubeObject::CheckCollision() const
{
		glm::vec3 targetVec = Camera::MainCamera.Position - transform.Location;
		float distanceSqr = targetVec.x * targetVec.x + targetVec.y * targetVec.y + targetVec.z * targetVec.z;
		
		return distanceSqr >= colliderRadius * colliderRadius * colliderRadius;
}

void StaticCubeObject::Initialize()
{
		objectShader.LoadShaderFromFile(
				"src/shaders/vertex/light_casters_vertex.glsl",
				"src/shaders/fragment/light_casters_fragment.glsl"
		);

		float vertices[] = {
				// positions          // normals           // texture coords
				-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,  1.0f,  1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f, 1.0f,  1.0f,  1.0f,
				 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
				 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,  1.0f,  1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,  1.0f,  1.0f,

				-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,  1.0f,  1.0f,
				 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f, 1.0f,  1.0f,  1.0f,
				 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
				 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,  1.0f,  1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,  1.0f,  1.0f,

				-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,  1.0f,  1.0f,
				-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
				-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, 1.0f,  1.0f,  1.0f,
				-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, 1.0f,  1.0f,  1.0f,
				-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, 1.0f,  1.0f,  1.0f,
				-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,  1.0f,  1.0f,

				 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,  1.0f,  1.0f,
				 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
				 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, 1.0f,  1.0f,  1.0f,
				 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, 1.0f,  1.0f,  1.0f,
				 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, 1.0f,  1.0f,  1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,  1.0f,  1.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  1.0f,  1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f,  1.0f,  1.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f,  1.0f,  1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  1.0f,  1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  1.0f,  1.0f,

				-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  1.0f,  1.0f,
				 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
				 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f,  1.0f,  1.0f,
				 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f,  1.0f,  1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  1.0f,  1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		};

		glGenVertexArrays(1, &objectVAO);
		glGenBuffers(1, &objectVBO);

		glBindVertexArray(objectVAO);
		glBindBuffer(GL_ARRAY_BUFFER, objectVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3);

		diffuseTexture = TextureFromFile("resources/textures/box_diffuse.png");
		specularTexture = TextureFromFile("resources/textures/box_specular.png");

		objectShader.UseShader();
		objectShader.SetIntParameter("material.diffuse", 0);
		objectShader.SetIntParameter("material.specular", 1);

		objectShader.SetBoolParameter("torchIsEnabled", false);
		objectShader.SetBoolParameter("dirIsEnabled", true);
		objectShader.SetBoolParameter("pointIsEnabled", true);

		Input::Subscribe(std::make_shared<Shader>(objectShader));
}

void StaticCubeObject::Render(glm::mat4 view, glm::mat4 projection)
{
		Object::SetupRender();

		// view/projection transformations
		objectShader.SetVec3Parameter("viewPos", Camera::MainCamera.Position);
		objectShader.SetMat4Parameter("projection", projection);
		objectShader.SetMat4Parameter("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		objectShader.SetMat4Parameter("model", model);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTexture);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularTexture);

		glBindVertexArray(objectVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
		glUseProgram(0);

		if (!CheckCollision())
		{
				glm::vec3 targetVec = Camera::MainCamera.Position - transform.Location;
				Camera::MainCamera.Position = glm::normalize(targetVec) * colliderRadius;
		}
}
