#pragma once

#include <memory>
#include <glm/ext.hpp>

#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "Texture.h"

class GameObject
{
public:
	GameObject();
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;

	void lookAlong(const glm::vec3& direction);
	void lookAt(const glm::vec3& center, const glm::vec3& up = glm::vec3(0, 1, 0));
	void rotate(const glm::quat& q);
	void preRotate(const glm::quat& q);
	glm::mat4 matrix() const;
	glm::vec3 forward() const;
	glm::vec3 right() const;
	glm::vec3 up() const;
	void render() const;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Light> light;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Texture> texture;
};

