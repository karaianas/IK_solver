#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 direction;

	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 pos;
in vec3 norm;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform bool isNormalMode;

void main()
{
	vec3 md = material.diffuse;
	vec3 ms = material.specular;
	vec3 ma = material.ambient;
	vec3 ld = light.diffuse;
	vec3 ls = light.specular;
	vec3 la = light.ambient;

	vec3 nColor = vec3(normalize(norm + 1.0f)/2.0f);

	if (isNormalMode)
		md = ms = ma = nColor;

	vec3 lightDir = normalize(-light.direction);

	float attenuation = 1.0f;

	// Diffuse
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = ld * md * diff;

	// Specular
	vec3 viewDir = normalize(viewPos - pos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = ls * ms * spec;

	// Ambient
	vec3 ambient = la * ma;

	// Attenuation
	diffuse  *= attenuation;
	specular *= attenuation;
	ambient  *= attenuation;

	vec3 result = ambient + diffuse + specular;

	color = vec4(result, 1.0f);
}