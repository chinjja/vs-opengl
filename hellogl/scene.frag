#version 330 core

struct DirectionalLight {
	vec4 color;
	vec3 direction;
	float intensity;
};

struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float reflectance;
};

uniform DirectionalLight directionalLight;
uniform Material material;
uniform vec3 cameraVertex;

in vec3 modelNormal;
in vec3 modelVertex;
out vec4 fragColor;

void main()
{
	float factor = clamp(dot(-directionalLight.direction, modelNormal), 0, 1);
	vec3 reflected_light = reflect(directionalLight.direction, modelNormal);
	vec3 to_camera_dir = normalize(cameraVertex - modelVertex);
	float specular_factor = clamp(dot(to_camera_dir, reflected_light), 0, 1) * material.reflectance;
	vec4 ambient = material.ambient * directionalLight.color;
	ambient += material.diffuse * 0.01;
	ambient += material.specular * 0.01;
	fragColor = (ambient * 0.2 + ambient * factor + ambient * specular_factor) * directionalLight.intensity;
}
