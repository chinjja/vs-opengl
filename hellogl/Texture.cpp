#include "Texture.h"

Texture::Texture()
	: ambient(1, 1, 1)
{
}

Texture::Texture(const glm::vec3& ambient)
	: ambient(ambient)
{
}
