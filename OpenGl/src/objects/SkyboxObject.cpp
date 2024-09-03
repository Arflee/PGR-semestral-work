#include "SkyboxObject.h"

void SkyboxObject::Initialize()
{
		objectShader.LoadShaderFromFile("src/shaders/vertex/skybox_vertex.glsl", "src/shaders/fragment/skybox_fragment.glsl");

		static const float screenCoords[] = {
				-1.0f, -1.0f,
				 1.0f, -1.0f,
				-1.0f,  1.0f,
				 1.0f,  1.0f
		};

		glGenVertexArrays(1, &objectVAO);
		glGenBuffers(1, &objectVBO);

		glBindVertexArray(objectVAO);
		glBindBuffer(GL_ARRAY_BUFFER, objectVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(screenCoords), screenCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		std::vector<std::string> faces
		{
				"resources/skybox/right.jpg",
				"resources/skybox/left.jpg",
				"resources/skybox/top.jpg",
				"resources/skybox/bottom.jpg",
				"resources/skybox/front.jpg",
				"resources/skybox/back.jpg"
		};

		stbi_set_flip_vertically_on_load(false);
		objectTexture = LoadCubemap(faces);
		stbi_set_flip_vertically_on_load(true);

		objectShader.UseShader();
		objectShader.SetIntParameter("skybox", 0);
}

void SkyboxObject::Render(glm::mat4 view, glm::mat4 projection)
{
		objectShader.UseShader();
		view = glm::mat4(glm::mat3(view));

		objectShader.SetMat4Parameter("inversePVmatrix", glm::inverse(projection * view));
		glBindVertexArray(objectVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, objectTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindVertexArray(0);
		glUseProgram(0);
}

