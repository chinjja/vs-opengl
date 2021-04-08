#pragma once

#include <memory>
#include <glm/ext.hpp>

#include "Mesh.h"

class GameObject
{
public:
	GameObject();
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;

	void rotate(const glm::quat& q);
	void preRotate(const glm::quat& q);
	glm::mat4 matrix() const;
	glm::vec3 forward() const;
	glm::vec3 right() const;
	glm::vec3 up() const;
	void render() const;
	std::shared_ptr<Mesh> mesh;
};

