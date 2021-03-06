#pragma once

#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class Mesh;

class ObjectLoader
{
public:
	static bool load(const std::string& filename, std::vector<glm::vec3>& vertex, std::vector<glm::vec3>& normal, std::vector<glm::vec2>& uv);
	static std::shared_ptr<Mesh> load(const std::string& filename);
};

