#pragma once

#include <glm/glm.hpp>

class Light
{
public:
	Light(const glm::vec4& color = glm::vec4(1), float intensity = 1);
	glm::vec4 color;
	float intensity;
};
