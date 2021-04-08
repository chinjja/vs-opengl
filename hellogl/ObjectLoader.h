#pragma once
#include "Mesh.h"

#include <memory>
#include <vector>
#include <glm/glm.hpp>

class ObjectLoader
{
public:
	static bool load(const char* filename, std::vector<glm::vec3>& vertex, std::vector<glm::vec3>& normal, std::vector<glm::vec2>& uv);
	static std::shared_ptr<Mesh> load(const char* filename);
};

