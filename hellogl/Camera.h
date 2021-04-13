#pragma once

#include <memory>
#include <glm/glm.hpp>

class GameObject;

class Camera
{
public:
	Camera();

	glm::mat4 matrix(const std::shared_ptr<GameObject>& obj) const;
	float fov;
	float near;
	float far;
	float ratio;
};

