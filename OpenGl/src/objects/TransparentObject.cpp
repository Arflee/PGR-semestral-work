#include "TransparentObject.h"
#include "../Input.h"
#include "../Camera.h"

void TransparentObject::Initialize()
{
		objectShader.LoadShaderFromFile(
				"src/shaders/vertex/transparent_vertex.glsl",
				"src/shaders/fragment/transparent_fragment.glsl"
		);

		float transparentVertices[] = {
				0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
				0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
				1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

				0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
				1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
				1.0f,  0.5f,  0.0f,  1.0f,  0.0f
		};

		// transparent VAO
		glGenVertexArrays(1, &objectVAO);
		glGenBuffers(1, &objectVBO); 

		glBindVertexArray(objectVAO);
		glBindBuffer(GL_ARRAY_BUFFER, objectVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0);

		objectTexture = TextureFromFile("resources/textures/window.png");

		objectShader.UseShader();
		objectShader.SetIntParameter("texture1", 0);
}

void TransparentObject::Render(glm::mat4 view, glm::mat4 projection)
{
		objectShader.UseShader();
		objectShader.SetMat4Parameter("projection", projection);
		objectShader.SetMat4Parameter("view", view);

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
}
