#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader();
	~Shader();

	bool addShaderCode(GLenum type, const char* code);
	bool addVertexShaderCode(const char* code);
	bool addFragmentShaderCode(const char* code);
	bool link();
	void bind();
	void release();

	GLint uniformLocation(const char* name);
	GLint attribLocation(const char* name);
	void setUniformValue(const char* name, const glm::mat4& mat);
	void setUniformValue(GLint location, const glm::mat4& mat);
private:
	GLuint id_;
	GLchar buf_[256];
	std::unordered_map<std::string, GLint> locations_;
	std::unordered_map<std::string, GLint> attrib_;
};

