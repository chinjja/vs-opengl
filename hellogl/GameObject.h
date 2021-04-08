#pragma once

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
	Mesh* mesh;
};

