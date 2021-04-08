#pragma once
#include <memory>
#include "Scene.h"
#include "GameObject.h"
#include "GameLogic.h"

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

