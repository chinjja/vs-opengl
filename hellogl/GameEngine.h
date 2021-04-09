#pragma once
#include <memory>
#include <GLFW/glfw3.h>
class GameLogic;

class GameEngine
{
public:
	GameEngine(std::shared_ptr<GameLogic>& logic, int width = 640, int height = 480);

private:
	void render();

private:
	std::shared_ptr<GameLogic> logic_;
	float prev_time_;
	GLFWwindow* window_;
};

