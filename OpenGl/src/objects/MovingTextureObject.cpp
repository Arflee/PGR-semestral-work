#include "MovingTextureObject.h"
#include "../Model.h"
#include "../stb_image.h"
#include <iostream>

bool MovingTextureObject::isDone = true;

void MovingTextureObject::Initialize()
{
		objectShader.LoadShaderFromFile(
				"src/shaders/vertex/snowflake_vertex.glsl",
				"src/shaders/fragment/snowflake_fragment.glsl"
		);

		float vertices[] = {
				-1.0f,  0.5f,  0.0f,  0.0f,  0.0f,
				-1.0f, -0.5f,  0.0f,  0.0f,  1.0f,
				1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

				-1.0f,  0.5f,  0.0f,  0.0f,  0.0f,
				1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
				1.0f,  0.5f,  0.0f,  1.0f,  0.0f
		};

		// transparent VAO
		glGenVertexArrays(1, &objectVAO);
		glGenBuffers(1, &objectVBO);

		glBindVertexArray(objectVAO);
		glBindBuffer(GL_ARRAY_BUFFER, objectVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0);

		std::string filename = std::string("resources/textures/snowflake.png");
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
				GLenum format;
				if (nrComponents == 1)
						format = GL_RED;
				else if (nrComponents == 3)
						format = GL_RGB;
				else if (nrComponents == 4)
						format = GL_RGBA;

				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

				stbi_image_free(data);

				objectTexture = textureID;
				objectShader.UseShader();
				objectShader.SetIntParameter("texture1", 0);
		}
}

float oldTimeSinceStart = 0;
float elapsedTime = 0;
float duration = 4.f;

void MovingTextureObject::Render(glm::mat4 view, glm::mat4 projection)
{
		if (isDone)
		{
				elapsedTime = 0;
				oldTimeSinceStart = glutGet(GLUT_ELAPSED_TIME);
				return;
		}

		int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
		int deltaTime = timeSinceStart - oldTimeSinceStart;
		oldTimeSinceStart = timeSinceStart;
		elapsedTime += deltaTime;

		objectShader.UseShader();
		const glm::mat4 orthoProjectionMatrix = glm::ortho(
				-1.f, 1.f,
				-1.f, 1.f,
				-10.0f * 1.f, 10.0f * 1.f
		);
		// static viewpoint - top view
		const glm::mat4 orthoViewMatrix = glm::lookAt(
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f)
		);
		objectShader.SetMat4Parameter("projection", orthoProjectionMatrix);
		objectShader.SetMat4Parameter("view", orthoViewMatrix);
		objectShader.SetFloatParameter("time", elapsedTime / 800);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, transform.Location);
		model = glm::scale(model, transform.Scale);
		model = glm::rotate(model, glm::radians(transform.RotationAngle), transform.RotationAxis);
		objectShader.SetMat4Parameter("model", model);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, objectTexture);

		glBindVertexArray(objectVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
		glUseProgram(0);


		if (elapsedTime >= duration * 1000)
		{
				isDone = true;
				elapsedTime = 0;
		}
}
