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

	bool addShaderCode(GLenum type, const std::string& code);
	bool addVertexShaderCode(const std::string& code);
	bool addFragmentShaderCode(const std::string& code);
	bool addShaderFile(GLenum type, const std::string& filename);
	bool addVertexShaderFile(const std::string& filename);
	bool addFragmentShaderFile(const std::string& filename);
	bool link();
	void bind();
	void release();

	GLint uniformLocation(const std::string& name);
	GLint attribLocation(const std::string& name);
	void setUniformValue(const std::string& name, const glm::mat4& mat);
	void setUniformValue(const std::string& name, float scalar);
	void setUniformValue(const std::string& name, const glm::vec2& vec);
	void setUniformValue(const std::string& name, const glm::vec3& vec);
	void setUniformValue(const std::string& name, const glm::vec4& vec);
	void setUniformValue(GLint location, const glm::mat4& mat);
	void setUniformValue(GLint location, float scalar);
	void setUniformValue(GLint location, const glm::vec2& vec);
	void setUniformValue(GLint location, const glm::vec3& vec);
	void setUniformValue(GLint location, const glm::vec4& vec);
private:
	GLuint id_;
	GLchar buf_[256];
	std::unordered_map<std::string, GLint> locations_;
	std::unordered_map<std::string, GLint> attrib_;
};

