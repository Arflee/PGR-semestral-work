#include <pgr.h>
#include <iostream>
#include "Input.h"
#include "Camera.h"
#include "Scene.h"
#include "objects/TextureAnimationObject.h"
#include "objects/EagleObject.h"
#include "objects/MeshAnimationObject.h"
#include "objects/MovingTextureObject.h"

float lastX = 0;
float lastY = 0;

bool firstMouse = true;
bool torchIsEnabled = false;
bool dirIsEnabled = true;
bool pointIsEnabled = true;
bool isStaticCamera = false;
bool isAligned = false;

int Input::deltaTime;
std::set<char> Input::pressedKeys;
std::vector<std::shared_ptr<Shader>> Input::subscribers;

void Input::SetDeltaTime(int delta)
{
    deltaTime = delta;
    UpdateControls();
}

void Input::PassiveMouseMotionCallback(int mouseX, int mouseY)
{
    if (firstMouse)
    {
        lastX = mouseX;
        lastY = mouseY;
        firstMouse = false;
    }

    float xoffset = mouseX - lastX;
    float yoffset = lastY - mouseY;

    lastX = mouseX;
    lastY = mouseY;

    Camera::MainCamera.ProcessMouseMovement(xoffset, yoffset);
    glutPostRedisplay();

    if (mouseX < 800 / 2 - 1 || mouseX > 800 / 2 + 2)
    {
        lastX = 800 / 2;
        lastY = 800 / 2;
        glutWarpPointer(800 / 2, 800 / 2);
    }
    if (mouseY < 800 / 2 - 2 || mouseY > 800 / 2 + 2)
    {
        lastX = 800 / 2;
        lastY = 800 / 2;
        glutWarpPointer(800 / 2, 800 / 2);
    }
}

void Input::KeyCallback(unsigned char keyPressed, int mouseX, int mouseY)
{
    if (keyPressed == 27)
    {
        glutLeaveMainLoop();
    }

    if (keyPressed == '1')
    {
        dirIsEnabled = !dirIsEnabled;
        for (auto i : subscribers)
        {
            i->UseShader();
            i->SetBoolParameter("dirIsEnabled", dirIsEnabled);
        }
    }
    if (keyPressed == '2')
    {
        pointIsEnabled = !pointIsEnabled;
        for (auto i : subscribers)
        {
            i->UseShader();
            i->SetBoolParameter("pointIsEnabled", pointIsEnabled);
        }
    }
    if (keyPressed == '3')
    {
        torchIsEnabled = !torchIsEnabled;
        for (auto i : subscribers)
        {
            i->UseShader();
            i->SetBoolParameter("torchIsEnabled", torchIsEnabled);
        }
    }
    
    pressedKeys.insert(keyPressed);
}

void Input::KeyUpCallback(unsigned char keyReleased, int mouseX, int mouseY)
{
    pressedKeys.erase(keyReleased);
}

void Input::SpecialKeyCallback(int specKeyPressed, int mouseX, int mouseY)
{
    if (specKeyPressed == GLUT_KEY_F1)
    {
        Camera::MainCamera.UnlockControls();
        isStaticCamera = false;
        isAligned = false;
        Camera::MainCamera.ProcessMouseMovement(0, 0);
    }
    else if(specKeyPressed == GLUT_KEY_F2)
    {
        Camera::MainCamera.Position = EagleObject::eaglePosition.Location;
        Camera::MainCamera.LockControls();
        isStaticCamera = true;
        isAligned = true;
        Camera::MainCamera.Pitch = -8.5;
        Camera::MainCamera.ProcessMouseMovement(0, 0);
    }
    else if(specKeyPressed == GLUT_KEY_F3)
    {
        Camera::MainCamera.UnlockControls();
        isAligned = false;

        Camera::MainCamera.Position = glm::vec3(-5.f, 3.f, 5.f);
        Camera::MainCamera.Yaw = -65.f;
        Camera::MainCamera.Pitch = -15.f;
        Camera::MainCamera.LockControls();
        isStaticCamera = true;
        Camera::MainCamera.ProcessMouseMovement(0, 0);
    }
}

void Input::SpecialKeyUpCallback(int specKeyReleased, int mouseX, int mouseY)
{
}

bool windmillIsStopped = false;

void Input::MouseCallback(int buttonPressed, int buttonState, int mouseX, int mouseY)
{
    if (buttonPressed == 3) // It's a wheel event
    {
        if (buttonState == GLUT_UP) return;

        Camera::MainCamera.ProcessMouseScroll(1.0f);
    }
    else if (buttonPressed == 4)
    {
        if (buttonState == GLUT_UP) return

        Camera::MainCamera.ProcessMouseScroll(-1.0f);
    }

    unsigned char ObjectID = 0;
    if (buttonPressed == GLUT_LEFT_BUTTON)
    {
        if (buttonState == GLUT_UP) return;

        glReadPixels(mouseX, mouseY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &ObjectID);
        if (ObjectID == 1)
        {
            TextureAnimationObject::SnapTorchToCamera();
        }
        else if (ObjectID == 9 || ObjectID == 8)
        {
            if (windmillIsStopped)
            {
                MeshAnimationObject::rotationSpeed = 0.055f;
                windmillIsStopped = false;
            }
            else
            {
                MeshAnimationObject::rotationSpeed = 0.f;
                windmillIsStopped = true;
            }
        }
        else if (ObjectID == 6)
        {
            MovingTextureObject::isDone = false;
        }
    }
}

void Input::UpdateControls()
{
    if (isAligned)
    {
        Camera::MainCamera.Position = EagleObject::eaglePosition.Location + glm::vec3(0.f, 0.15f, 0.f);
        Camera::MainCamera.Yaw = EagleObject::eaglePosition.RotationAngle + 90.f;
        Camera::MainCamera.ProcessMouseMovement(0, 0);
    }

    if (isStaticCamera) return;

    Camera::MainCamera.Position.y = glm::clamp(Camera::MainCamera.Position.y, -0.5f, 6.f);
    Camera::MainCamera.Position.x = glm::clamp(Camera::MainCamera.Position.x, -15.f, 15.f);
    Camera::MainCamera.Position.z = glm::clamp(Camera::MainCamera.Position.z, -15.f, 15.f);

    if (!pressedKeys.size()) return;

    float velocity = Camera::MainCamera.MovementSpeed * deltaTime;
    velocity /= 1000;

    if (pressedKeys.count('w') != 0)
    {
        Camera::MainCamera.Position += Camera::MainCamera.Front * velocity;
    }
    else if (pressedKeys.count('s') != 0)
    {
        Camera::MainCamera.Position -= Camera::MainCamera.Front * velocity;
    }
    if (pressedKeys.count('a') != 0)
    {
        Camera::MainCamera.Position -= Camera::MainCamera.Right * velocity;
    }
    else if (pressedKeys.count('d') != 0)
    {
        Camera::MainCamera.Position += Camera::MainCamera.Right * velocity;
    }
}

void Input::Subscribe(std::shared_ptr<Shader> subscriber)
{
    subscribers.push_back(subscriber);
}
