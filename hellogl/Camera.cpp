#include "Camera.h"
#include "GameObject.h"
#include <glm/ext.hpp>

using namespace std;
using namespace glm;

Camera::Camera()
{
}

glm::mat4 Camera::matrix(const std::shared_ptr<GameObject>& obj) const
{
	glm::mat4 m(1);

	quat r1(1, vec3());
	quat r2 = r1;
	std::shared_ptr<const GameObject> cur = obj;
	while (cur) {
		r1 = cur->rotation * r1;
		r2 = cur->localRotation * r2;
		cur = cur->parent().lock();
	}

	mat4 mat(1);
	mat[3] = obj->global()[3];
	mat *= mat4_cast(r1 * r2);
	return inverse(mat);
}
