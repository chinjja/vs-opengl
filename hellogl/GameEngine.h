#pragma once

class GameLogic;

class GameEngine
{
public:
	GameEngine(GameLogic* logic, int width = 640, int height = 480);

private:
	GameLogic* logic_;
};

