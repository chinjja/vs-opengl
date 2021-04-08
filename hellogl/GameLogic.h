#pragma once

class GameEngine;

class GameLogic
{
protected:
	virtual void init() = 0;
	virtual void update() = 0;
	virtual void input() = 0;

	float width;
	float height;
	float time;
	float delta_time;

public:
	friend class GameEngine;
};

