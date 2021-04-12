#pragma once

#include <memory>
#include <vector>
#include <glm/ext.hpp>

#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	const static glm::vec3 FORWARD;
	const static glm::vec3 RIGHT;
	const static glm::vec3 UP;

public:
	static std::shared_ptr<GameObject> create();
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;

	void lookAlong(const glm::vec3& direction, const glm::vec3& bais = FORWARD);
	void lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up = UP);
	void rotate(const glm::quat& q);
	void preRotate(const glm::quat& q);
	glm::mat4 global(bool disable_scale = false) const;
	glm::mat4 local(bool disable_scale = false) const;
	glm::vec3 forward() const;
	glm::vec3 right() const;
	glm::vec3 up() const;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Light> light;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Material> material;
	
	bool addChild(std::shared_ptr<GameObject>& child);
	bool removeChild(std::shared_ptr<GameObject>& child);
	std::weak_ptr<GameObject> parent() const;
	
	void getChildren(std::vector<std::shared_ptr<GameObject>>& result, bool recusive = true);
private:
	GameObject();
	std::weak_ptr<GameObject> parent_;
	std::vector<std::shared_ptr<GameObject>> children_;
};

