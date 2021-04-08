#pragma once

#include <glm/glm.hpp>

class Texture
{
public:
	Texture();
	Texture(const glm::vec3& ambient);
	glm::vec3 ambient;
};

