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

	glm::mat4 matrix();
	void render();
	std::shared_ptr<Mesh> mesh;
};

