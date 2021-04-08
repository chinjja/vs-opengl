#pragma once
#include <memory>
class GameLogic;

class GameEngine
{
public:
	GameEngine(std::shared_ptr<GameLogic>& logic, int width = 640, int height = 480);

private:
	std::shared_ptr<GameLogic> logic_;
};

