#include "GameEngine.h"
#include "GameLogicImpl.h"

int main()
{
    GameLogicImpl* impl = new GameLogicImpl;
    GameEngine engine(impl);
    return EXIT_SUCCESS;
}