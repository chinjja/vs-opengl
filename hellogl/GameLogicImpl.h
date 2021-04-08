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
    void input() override;

private:
    Scene scene;
    std::shared_ptr<GameObject> cube1;
};

