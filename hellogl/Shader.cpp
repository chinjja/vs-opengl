#include "Shader.h"

#include <cstdio>
#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

Shader::Shader()
{
	id_ = glCreateProgram();
	memset(buf_, 0, sizeof(buf_));
}

Shader::~Shader()
{
	glDeleteProgram(id_);
}

bool Shader::addShaderCode(GLenum type, const char* code)
{
	GLint st;
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &st);
	if (!st) {
		GLint len;
		glGetShaderInfoLog(shader, sizeof(buf_), &len, buf_);
		std::cout << std::string(buf_, len) << std::endl;
	}
	else {
		glAttachShader(id_, shader);
	}
	glDeleteShader(shader);
	return st;
}

bool Shader::addVertexShaderCode(const char* code)
{
	return addShaderCode(GL_VERTEX_SHADER, code);
}

bool Shader::addFragmentShaderCode(const char* code)
{
	return addShaderCode(GL_FRAGMENT_SHADER, code);
}

bool Shader::addShaderFile(GLenum type, const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (!file) {
		std::cout << "can't open file: " << filename << std::endl;
		return false;
	}
	std::string source;
	char buf[2048];
	while (!feof(file)) {
		int len = fread(buf, 1, sizeof(buf), file);
		source += std::string(buf, len);
	}
	fclose(file);
	return addShaderCode(type, source.c_str());
}

bool Shader::addVertexShaderFile(const char* filename)
{
	return addShaderFile(GL_VERTEX_SHADER, filename);
}

bool Shader::addFragmentShaderFile(const char* filename)
{
	return addShaderFile(GL_FRAGMENT_SHADER, filename);
}

bool Shader::link()
{
	GLint i;
	glLinkProgram(id_);
	glGetProgramiv(id_, GL_LINK_STATUS, &i);
	if (!i) {
		glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &i);
		glGetProgramInfoLog(id_, sizeof(buf_), &i, buf_);
		std::cout << std::string(buf_, i) << std::endl;
		return false;
	}
	return true;
}

void Shader::bind()
{
	glUseProgram(id_);
}

void Shader::release()
{
	glUseProgram(0);
}

GLint Shader::uniformLocation(const char* name)
{
	GLint location;
	auto it = locations_.find(name);
	if (it == locations_.end()) {
		location = glGetUniformLocation(id_, name);
		assert(location >= 0);
		locations_[name] = location;
	}
	else {
		location = (*it).second;
	}
	return location;
}

GLint Shader::attribLocation(const char* name)
{
	GLint attrib;
	auto it = attrib_.find(name);
	if (it == attrib_.end()) {
		attrib = glGetAttribLocation(id_, name);
		assert(attrib >= 0);
		attrib_[name] = attrib;
	}
	else {
		attrib = (*it).second;
	}
	return attrib;
}

void Shader::setUniformValue(const char* name, const glm::mat4& mat)
{
	setUniformValue(uniformLocation(name), mat);
}

void Shader::setUniformValue(const char* name, float scalar)
{
	setUniformValue(uniformLocation(name), scalar);
}

void Shader::setUniformValue(const char* name, const glm::vec2& vec)
{
	setUniformValue(uniformLocation(name), vec);
}

void Shader::setUniformValue(const char* name, const glm::vec3& vec)
{
	setUniformValue(uniformLocation(name), vec);
}

void Shader::setUniformValue(const char* name, const glm::vec4& vec)
{
	setUniformValue(uniformLocation(name), vec);
}

void Shader::setUniformValue(GLint location, const glm::mat4& mat)
{
	glProgramUniformMatrix4fv(id_, location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setUniformValue(GLint location, float scalar)
{
	glProgramUniform1fv(id_, location, 1, &scalar);
}

void Shader::setUniformValue(GLint location, const glm::vec2& vec)
{
	glProgramUniform2fv(id_, location, 1, &vec[0]);
}

void Shader::setUniformValue(GLint location, const glm::vec3& vec)
{
	glProgramUniform3fv(id_, location, 1, &vec[0]);
}

void Shader::setUniformValue(GLint location, const glm::vec4& vec)
{
	glProgramUniform4fv(id_, location, 1, &vec[0]);
}
