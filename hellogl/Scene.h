#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

class Shader;
class GameObject;

class Scene
{
public:
	void add(const std::shared_ptr<GameObject>& obj);
	void remove(const std::shared_ptr<GameObject>& obj);
	void render();
	glm::mat4 projection;
	std::shared_ptr<Shader> shader;
private:
	std::vector<std::shared_ptr<GameObject>> gameObjects_;
};

