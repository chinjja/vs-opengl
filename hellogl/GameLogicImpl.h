#pragma once
#include <memory>
#include "GameLogic.h"
#include "Scene.h"

class GameObject;

class GameLogicImpl : public GameLogic
{
protected:
    void init() override;
    void update() override;

private:
    Scene scene;
    std::shared_ptr<GameObject> cube1;
	std::shared_ptr<GameObject> head;
    std::shared_ptr<GameObject> player;
    std::shared_ptr<GameObject> child2;
    std::shared_ptr<GameObject> cube2;
};

