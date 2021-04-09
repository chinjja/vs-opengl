#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Texture
{
public:
	Texture(int width, int height, GLenum pixelFormat);
	Texture(const std::string& filename);
	~Texture();
	void bind();
	void release();
	int width();
	int height();

private:
	GLuint id_;
	int width_;
	int height_;
};

