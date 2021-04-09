#include "GameObject.h"
#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "Texture.h"

#include <algorithm>

using namespace std;
using namespace glm;

const glm::vec3 GameObject::FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 GameObject::RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 GameObject::UP = glm::vec3(0.0f, 1.0f, 0.0f);

GameObject::GameObject()
    : position(0, 0, 0),
    scale(1, 1, 1),
    rotation(1, 0, 0, 0),
    mesh(nullptr)
{
}

void GameObject::lookAlong(const glm::vec3& direction, const glm::vec3& bais)
{
    vec3 b = normalize(bais);
    vec3 v = normalize(direction);
    float angle = acosf(dot(b, v));
    vec3 axis = cross(b, v);
    rotation = angleAxis(angle, axis);
}

void GameObject::lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
{
    glm::mat4 m = inverse(glm::lookAt(eye, center, normalize(up)));
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

glm::mat4 GameObject::global(bool disable_scale) const
{
    const GameObject* cur = this;
    glm::mat4 m = cur->local(disable_scale);
    while (cur->parent()) {
		cur = cur->parent();
        m = cur->local(disable_scale) * m;
    }
    return m;
}

glm::mat4 GameObject::local(bool disable_scale) const
{
	glm::vec3 factor(1, 1, 1);
	if(!disable_scale && parent()) {
		factor = 1.0f / parent()->scale;
	}
    glm::mat4 ret(1);
	ret = translate(ret, position * factor);
    ret *= mat4_cast(rotation);
	if(!disable_scale)
		ret = glm::scale(ret, scale);
    return ret;
}

glm::vec3 GameObject::forward() const
{
    return rotation * FORWARD;
}

glm::vec3 GameObject::right() const
{
    return rotation * RIGHT;
}

glm::vec3 GameObject::up() const
{
    return rotation * UP;
}

GameObject* GameObject::parent() const
{
    return parent_;
}

void GameObject::getChildren(std::vector<GameObject *> &result, bool recursive) {
	for(auto child : children_) {
		if(recursive && !child->children_.empty()) {
			child->getChildren(result, recursive);
		}
		result.push_back(child.get());
	}
}

bool GameObject::removeChild(std::shared_ptr<GameObject> &child) {
	if(child.get() == this || child->parent_ != this) return false;
	
	auto it = find(children_.begin(), children_.end(), child);
	if(it != children_.end())
		children_.erase(it);
	return true;
}


bool GameObject::addChild(std::shared_ptr<GameObject> &child) {
	if(child.get() == this || child->parent_ == this) return false;
	
	if(child->parent_) {
		child->parent_->removeChild(child);
	}
	child->parent_ = this;
	children_.push_back(child);
	return true;
}
