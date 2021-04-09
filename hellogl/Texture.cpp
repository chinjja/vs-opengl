#include "Texture.h"

#include <string>
#include <exception>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(int width, int height, GLenum pixelFormat)
	: width_(width), height_(height)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &id_);
	assert(id_ > 0);

	glBindTexture(GL_TEXTURE_2D, id_);
	glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, width, height, 0, pixelFormat, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Texture::Texture(const std::string& filename)
{
	int channels;
	stbi_uc* data = stbi_load(filename.c_str(), &width_, &height_, &channels, 0);
	if (!data) {
		throw std::exception(("can't load image: " + std::string(filename)).c_str());
	}
	glCreateTextures(GL_TEXTURE_2D, 1, &id_);
	assert(id_ > 0);
	glBindTexture(GL_TEXTURE_2D, id_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_FLOAT, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id_);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::release()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::width()
{
	return width_;
}

int Texture::height()
{
	return height_;
}
