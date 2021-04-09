#version 330 core
layout(location = 0) in vec3 vertex_data;
layout(location = 1) in vec3 normal_data;

uniform mat4 M;
uniform mat4 MVP;

out vec3 modelNormal;
out vec3 modelVertex;

void main()
{
	gl_Position = MVP * vec4(vertex_data, 1);
	modelNormal = normalize((M * vec4(normal_data, 0)).xyz);
	modelVertex = (M * vec4(vertex_data, 1)).xyz;
}
