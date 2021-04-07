#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Mesh
{
public:
	Mesh(const std::vector<glm::vec3> &vertex, const std::vector<glm::vec3> &normal, const std::vector<glm::vec2> &uv);
	~Mesh();
	void bind();
	void release();
	void render();

private:
	GLuint vao_;
	GLuint vbo_[3];
	GLsizei count_;
};

