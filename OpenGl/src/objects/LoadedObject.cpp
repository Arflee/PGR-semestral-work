#include "LoadedObject.h"
#include "../Camera.h"
#include "../Input.h"
#include "TextureAnimationObject.h"

void LoadedObject::Initialize()
{
		objectShader.LoadShaderFromFile(
        "src/shaders/vertex/" + shaderName + "_vertex.glsl",
        "src/shaders/fragment/" + shaderName + "_fragment.glsl"
    );
		meshModel = Model(std::string("resources/models/" + meshName).c_str(), flippedUV);

		objectShader.UseShader();
		objectShader.SetBoolParameter("torchIsEnabled", false);
		objectShader.SetBoolParameter("dirIsEnabled", true);
		objectShader.SetBoolParameter("pointIsEnabled", true);

		Input::Subscribe(std::make_shared<Shader>(objectShader));

		glBindVertexArray(0);
		glUseProgram(0);
}

void LoadedObject::Render(glm::mat4 view, glm::mat4 projection)
{
		LoadedObject::SetupRender();

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, fogTexture);

		// view/projection transformations
		objectShader.SetMat4Parameter("projection", projection);
		objectShader.SetMat4Parameter("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, transform.Location);
		model = glm::scale(model, transform.Scale);
		model = glm::rotate(model, glm::radians(transform.RotationAngle), transform.RotationAxis);
		objectShader.SetMat4Parameter("model", model);
    meshModel.Draw(objectShader);

		glBindVertexArray(0);
		glUseProgram(0);
}
