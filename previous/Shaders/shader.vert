#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 pos;
out vec3 norm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	pos = vec3(model * vec4(position, 1.0f));
	norm = normalize(mat3(transpose(inverse(model))) * normal);
	gl_Position = proj* view * model * vec4(position, 1.0f);
}