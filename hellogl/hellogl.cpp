#include "GameEngine.h"
#include "GameLogicImpl.h"

int main()
{
    std::shared_ptr<GameLogic> impl = std::make_shared<GameLogicImpl>();
    GameEngine engine(impl);
    return EXIT_SUCCESS;
}