#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Texture.h"

class Material
{
public:
	Material(const glm::vec4& ambient = glm::vec4(1), const glm::vec4& diffuse = glm::vec4(1), const glm::vec4& specular = glm::vec4(1), float reflectance = 0.2);
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	float reflectance;
	std::shared_ptr<Texture> texture;
};

