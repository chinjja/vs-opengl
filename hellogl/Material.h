#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Texture.h"

class Material
{
public:
	Material(const glm::vec3& ambient = glm::vec3(1, 1, 1));
	glm::vec3 ambient;
	std::shared_ptr<Texture> texture;
};

