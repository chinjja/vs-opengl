#include "GameObject.h"

GameObject::GameObject()
    : position(0, 0, 0),
    scale(1, 1, 1),
    rotation(1, 0, 0, 0),
    mesh(nullptr)
{
}

glm::mat4 GameObject::matrix()
{
    glm::mat4 ret(1);
    ret = glm::translate(ret, position);
    ret *= mat4_cast(rotation);
    ret = glm::scale(ret, scale);
    return ret;
}

void GameObject::render()
{
    mesh->bind();
    mesh->render();
    mesh->release();
}
