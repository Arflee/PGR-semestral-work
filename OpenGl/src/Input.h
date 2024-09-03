#pragma once
#include <set>
#include "Shader.h"

class Input
{
private:
		static int deltaTime;
		static std::set<char> pressedKeys;
		static std::vector<std::shared_ptr<Shader>> subscribers;

public:
		static void SetDeltaTime(int delta);
		static void PassiveMouseMotionCallback(int mouseX, int mouseY);
		static void KeyCallback(unsigned char keyPressed, int mouseX, int mouseY);
		static void KeyUpCallback(unsigned char keyReleased, int mouseX, int mouseY);
		static void SpecialKeyCallback(int specKeyPressed, int mouseX, int mouseY);
		static void SpecialKeyUpCallback(int specKeyReleased, int mouseX, int mouseY);
		static void MouseCallback(int buttonPressed, int buttonState, int mouseX, int mouseY);
		static void UpdateControls();
		static void Subscribe(std::shared_ptr<Shader> subscriber);
};