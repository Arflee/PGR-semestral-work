#include <iostream>
#include <pgr.h>
#include "Scene.h"
#include "Transform.h"
#include "Model.h"
#include "Camera.h"
#include "stb_image.h"
#include "Input.h"
#include "objects/SkyboxObject.h"
#include "objects/StaticCubeObject.h"
#include "objects/LoadedObject.h"
#include "objects/TextureAnimationObject.h"
#include "objects/EagleObject.h"
#include "objects/MeshAnimationObject.h"
#include "objects/TransparentObject.h"
#include "objects/MovingTextureObject.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int oldTimeSinceStart;

std::vector<std::shared_ptr<Object>> sceneObjects = std::vector<std::shared_ptr<Object>>();
std::shared_ptr<LoadedObject> torchPtr;

static void AddObjectsToScene()
{
		LoadedObject torch = LoadedObject("Torch.obj", "light_casters", true);
		torch.transform = Transform(glm::vec3(3.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.5f));
		torch.transform.RotationAngle = -180;
		torchPtr = std::make_shared<LoadedObject>(torch);

		LoadedObject christmasTree = LoadedObject("christmasTree.obj", "light_casters", false);
		christmasTree.transform = Transform(glm::vec3(-3.f, -1.f, 8.f), glm::vec3(1.f), glm::vec3(0.05f));

		LoadedObject snowman = LoadedObject("snehulak.obj", "light_casters", false);
		snowman.transform = Transform(glm::vec3(-3.f, -1.f, 0.f), glm::vec3(1.f), glm::vec3(0.03f));
		
		TransparentObject window = TransparentObject();
		window.transform = Transform(glm::vec3(-10.f, 0.f, 8.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.2f));
		window.transform.RotationAngle = -25;

		LoadedObject cloud = LoadedObject("cloud.stl", "light_casters", false);
		cloud.transform = Transform(glm::vec3(0.f, 3.f, 0.f), glm::vec3(1.f), glm::vec3(0.1f));

		LoadedObject cottage = LoadedObject("Snow covered CottageOBJ.obj", "light_casters", false);
		cottage.transform = Transform(glm::vec3(9.f, -0.8f, -9.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.1f));
		cottage.transform.RotationAngle = 120;

		LoadedObject windmillBody = LoadedObject("windmillBody.obj", "light_casters", false);
		windmillBody.transform = Transform(glm::vec3(-3.f, 3.f, -8.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.1f));
		windmillBody.transform.RotationAngle = -75;

		MeshAnimationObject windmillRotor = MeshAnimationObject("windmillRotor.obj", "light_casters", false);
		windmillRotor.transform = Transform(glm::vec3(-3.f, 4.62f, -8.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.1f));
		windmillRotor.transform.RotationAngle = -75;

		LoadedObject terrain = LoadedObject("testplane.obj", "light_casters", false);
		terrain.transform = Transform(glm::vec3(0.f, -1.f, 0.f), glm::vec3(1.f), glm::vec3(20.f));
		terrain.SetMaterial(Material(glm::vec3(1.f, 1.f, 0.f), glm::vec3(1.f), glm::vec3(0.03f), 32.0));

		EagleObject eagle = EagleObject("orol.3DS", "light_casters");
		eagle.transform = Transform(glm::vec3(-3.f, 2.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.01f));
		eagle.transform.RotationAngle = -90;

		TextureAnimationObject glowingFire = TextureAnimationObject();
		glowingFire.transform = Transform(torch.transform.Location + glm::vec3(0, 1.1f, 0.f), glm::vec3(1.f), glm::vec3(1.f));
		
		MovingTextureObject snowflake = MovingTextureObject();
		snowflake.transform = Transform(glm::vec3(0.f, 0.25f, 0.f), glm::vec3(1.f), glm::vec3(0.5f));

		sceneObjects.push_back(torchPtr);
		sceneObjects.push_back(std::make_shared<LoadedObject>(terrain));
		sceneObjects.push_back(std::make_shared<EagleObject>(eagle));
		sceneObjects.push_back(std::make_shared<LoadedObject>(snowman));
		sceneObjects.push_back(std::make_shared<LoadedObject>(cloud));
		sceneObjects.push_back(std::make_shared<LoadedObject>(christmasTree));
		sceneObjects.push_back(std::make_shared<LoadedObject>(cottage));
		sceneObjects.push_back(std::make_shared<LoadedObject>(windmillBody));
		sceneObjects.push_back(std::make_shared<MeshAnimationObject>(windmillRotor));
		sceneObjects.push_back(std::make_shared<SkyboxObject>(SkyboxObject()));
		sceneObjects.push_back(std::make_shared<TextureAnimationObject>(glowingFire));
		sceneObjects.push_back(std::make_shared<StaticCubeObject>(StaticCubeObject()));
		sceneObjects.push_back(std::make_shared<MovingTextureObject>(snowflake));
		sceneObjects.push_back(std::make_shared<TransparentObject>(window));
}

void Scene::InitializeApplication()
{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		stbi_set_flip_vertically_on_load(true);
		Camera::CreateCamera(glm::vec3(0.0f, 0.0f, 3.0f));

		AddObjectsToScene();

		// Apply translation
		for (size_t i = 0; i < sceneObjects.size(); i++)
		{
				sceneObjects[i]->Initialize();
		}
}


void Scene::TimerCallback(int)
{
		glutTimerFunc(33, TimerCallback, 0);
		glutPostRedisplay();
}

void Scene::DisplayCallback()
{
		glClearColor(0.1f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearStencil(0);

		int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
		int deltaTime = timeSinceStart - oldTimeSinceStart;
		oldTimeSinceStart = timeSinceStart;
		Input::SetDeltaTime(deltaTime);

		glm::mat4 projection = glm::perspective(glm::radians(Camera::MainCamera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = Camera::MainCamera.GetViewMatrix();

		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		if (TextureAnimationObject::isSnapped)
		{
				torchPtr->transform.Location = TextureAnimationObject::torchPosition + glm::vec3(0.0f, -1.f, 0.0f);
		}
		for (size_t i = 0; i < sceneObjects.size(); i++)
		{
				glStencilFunc(GL_ALWAYS, i + 1, -1);
				sceneObjects[i]->Render(view, projection);
		}

		glutSwapBuffers();
		glutPostRedisplay();
}