#pragma once

#include "Input.h"

class GameEngine;

class GameLogic
{
protected:
	virtual void init() = 0;
	virtual void update() = 0;

	float width;
	float height;
	float time;
	float delta_time;

	const Input& input() const;
private:
	Input input_;
public:
	friend class GameEngine;
};

