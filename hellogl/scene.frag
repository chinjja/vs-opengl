#version 330 core

struct DirectionalLight {
	vec3 direction;
	float intensity;
};

uniform DirectionalLight directionalLight;
uniform vec3 ambient;

in vec3 modelNormal;
out vec4 fragColor;

void main()
{
	float factor = clamp(dot(-directionalLight.direction, modelNormal), 0, 1) * directionalLight.intensity;
	fragColor = vec4(ambient * 0.2 + ambient * factor, 1);
}
