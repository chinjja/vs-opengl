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
    :
	position(0, 0, 0),
	scale(1, 1, 1),
	rotation(1, 0, 0, 0),
	localPosition(0, 0, 0),
	localScale(1, 1, 1),
	localRotation(1, 0, 0, 0),
    mesh(nullptr),
	light(nullptr),
	camera(nullptr),
	material(nullptr)
{
}

std::shared_ptr<GameObject> GameObject::create()
{
	return shared_ptr<GameObject>(new GameObject);
}

void GameObject::lookAlong(const glm::vec3& direction, const glm::vec3& bais)
{
    vec3 b = normalize(bais);
    vec3 v = normalize(direction);
    float angle = acosf(dot(b, v));
    vec3 axis = cross(b, v);
    localRotation = angleAxis(angle, axis);
}

void GameObject::lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
{
    glm::mat4 m = inverse(glm::lookAt(eye, center, normalize(up)));
    localPosition = m[3];
    localRotation = glm::quat(m);
}

void GameObject::rotate(const glm::quat& q)
{
    localRotation *= q;
}

void GameObject::preRotate(const glm::quat& q)
{
    localRotation = q * localRotation;
}

glm::mat4 GameObject::global() const
{
	vec3 p(0, 0, 0);
	quat r(1, vec3());
	vec3 s(1, 1, 1);
	
	std::shared_ptr<const GameObject> cur = shared_from_this();
    while (cur) {
		p += cur->position;
		r = cur->rotation * r;
		s *= cur->scale;
		
		cur = cur->parent_.lock();
    }
	return translate(mat4(1), p) * mat4_cast(r) * glm::scale(mat4(1), s) * local();
}

glm::mat4 GameObject::local() const
{
	mat4 m(1);
	std::shared_ptr<const GameObject> cur = shared_from_this();
	while (cur) {
		m = translate(mat4(1), cur->localPosition) * mat4_cast(cur->localRotation) * glm::scale(mat4(1), cur->localScale) * m;
		cur = cur->parent_.lock();
	}
	return m;
}

glm::vec3 GameObject::forward() const
{
    return localRotation * FORWARD;
}

glm::vec3 GameObject::right() const
{
    return localRotation * RIGHT;
}

glm::vec3 GameObject::up() const
{
    return localRotation * UP;
}

std::weak_ptr<GameObject> GameObject::parent() const
{
    return parent_;
}

void GameObject::getChildren(std::vector<std::shared_ptr<GameObject>> &result, bool recursive) {
	for(auto& child : children_) {
		if(recursive && !child->children_.empty()) {
			child->getChildren(result, recursive);
		}
		result.push_back(child);
	}
}

bool GameObject::removeChild(std::shared_ptr<GameObject> &child) {
	if(child.get() == this || child->parent_.lock().get() != this) return false;
	
	auto it = find(children_.begin(), children_.end(), child);
	if (it != children_.end()) {
		children_.erase(it);
		(*it)->parent_.reset();
	}
	return true;
}


bool GameObject::addChild(std::shared_ptr<GameObject> &child) {
	if(child.get() == this || child->parent_.lock().get() == this) return false;
	
	if(!child->parent_.expired()) {
		parent_.lock()->removeChild(child);
	}
	child->parent_ = weak_from_this();
	children_.push_back(child);
	return true;
}
