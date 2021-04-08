#include "GameObject.h"

GameObject::GameObject()
    : position(0, 0, 0),
    scale(1, 1, 1),
    rotation(1, 0, 0, 0),
    mesh(nullptr)
{
}

void GameObject::rotate(const glm::quat& q)
{
    rotation *= q;
}

void GameObject::preRotate(const glm::quat& q)
{
    rotation = q * rotation;
}

glm::mat4 GameObject::matrix() const
{
    glm::mat4 ret(1);
    ret = glm::translate(ret, position);
    ret *= mat4_cast(rotation);
    ret = glm::scale(ret, scale);
    return ret;
}

glm::vec3 GameObject::forward() const
{
    return rotation * glm::vec3(0, 0, -1);
}

glm::vec3 GameObject::right() const
{
    return rotation * glm::vec3(1, 0, 0);
}

glm::vec3 GameObject::up() const
{
    return rotation * glm::vec3(0, 1, 0);
}

void GameObject::render() const
{
    mesh->bind();
    mesh->render();
    mesh->release();
}
