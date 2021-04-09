#include "Material.h"

Material::Material(const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, float reflectance)
	: ambient(ambient), diffuse(diffuse), specular(specular), reflectance(reflectance)
{
}
