#pragma once

#include <memory>
#include <vector>
#include <glm/ext.hpp>

#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"

class GameObject
{
public:
	GameObject();
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;

	void lookAlong(const glm::vec3& direction, const glm::vec3& bais = glm::vec3(0, 0, -1));
	void lookAt(const glm::vec3& center, const glm::vec3& up = glm::vec3(0, 1, 0));
	void rotate(const glm::quat& q);
	void preRotate(const glm::quat& q);
	glm::mat4 global() const;
	glm::mat4 local() const;
	glm::vec3 forward() const;
	glm::vec3 right() const;
	glm::vec3 up() const;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Light> light;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Material> material;

	void setParent(std::shared_ptr<GameObject>& parent);
	const std::shared_ptr<GameObject>& parent() const;
private:
	std::shared_ptr<GameObject> parent_;
	std::vector<GameObject*> children_;
};

