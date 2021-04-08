#include "GameObject.h"
#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "Texture.h"

using namespace glm;

GameObject::GameObject()
    : position(0, 0, 0),
    scale(1, 1, 1),
    rotation(1, 0, 0, 0),
    mesh(nullptr)
{
}

void GameObject::lookAlong(const glm::vec3& direction)
{
    vec3 bias(0, 0, -1);
    float angle = acosf(dot(bias, direction));
    vec3 axis = cross(bias, direction);
    rotation = angleAxis(angle, axis);
}

void GameObject::lookAt(const glm::vec3& center, const glm::vec3& up)
{
    glm::mat4 m = glm::lookAt(position, center, up);
    position = m[3];
    rotation = glm::quat(m);
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
