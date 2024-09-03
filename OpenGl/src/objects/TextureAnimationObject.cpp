#include "TextureAnimationObject.h"
#include <iostream>
#include "../Camera.h"

glm::vec3 TextureAnimationObject::torchPosition;
bool TextureAnimationObject::isSnapped;

void TextureAnimationObject::Initialize()
{
    objectShader.LoadShaderFromFile("src/shaders/vertex/flame_vertex.glsl", "src/shaders/fragment/flame_fragment.glsl");
    objectTexture = TextureFromFile("resources/textures/flame.png");

    glGenVertexArrays(1, &objectVAO);
    glBindVertexArray(objectVAO);
    const int explosionNumQuadVertices = 4;
    const float explosionVertexData[explosionNumQuadVertices * 5] = {

        // x      y     z     u     v
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    };
    glGenBuffers(1, &objectVBO);
    glBindBuffer(GL_ARRAY_BUFFER, objectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(explosionVertexData), explosionVertexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    objectShader.UseShader();
    objectShader.SetIntParameter("texSampler", 0);
    objectShader.SetFloatParameter("frameDuration", frameDuration);
    torchPosition = transform.Location;
    glBindVertexArray(0);
}

void TextureAnimationObject::Render(glm::mat4 view, glm::mat4 projection)
{
    if (isSnapped)
    {
        transform.Location = Camera::MainCamera.Position + glm::vec3(1.2f, 0.1f, -0.6f);
        torchPosition = transform.Location;
    }

    int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    int deltaTime = timeSinceStart - oldTimeSinceStart;
    oldTimeSinceStart = timeSinceStart;
    elapsedTime += deltaTime;

    objectShader.UseShader();

    glm::mat4 billboardRotationMatrix = glm::mat4(
        view[0],
        view[1],
        view[2],
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );
    // inverse view rotation
    billboardRotationMatrix = glm::transpose(billboardRotationMatrix);

    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, transform.Location);
    matrix = glm::scale(matrix, glm::vec3(transform.Scale));

    matrix = matrix * billboardRotationMatrix;
    glm::mat4 PVMmatrix = projection * view;

    objectShader.SetMat4Parameter("model", matrix);
    objectShader.SetMat4Parameter("PVMmatrix", PVMmatrix);
    objectShader.SetFloatParameter("time", elapsedTime / 1000);
    glBindVertexArray(objectVAO);
    glBindTexture(GL_TEXTURE_2D, objectTexture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
    glUseProgram(0);

    if (elapsedTime / 1000 > totalDuration)
    {
        elapsedTime = 0.0f;
    }
}

void TextureAnimationObject::SnapTorchToCamera()
{
    isSnapped = true;
}
