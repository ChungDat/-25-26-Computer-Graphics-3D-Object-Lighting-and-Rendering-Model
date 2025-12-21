#version 330 core

struct Material {
	// ambient is usually set to the same as diffuse
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;

	float shininess;
};

uniform Material material;

in vec3 lightingColor;
in vec2 texCoord;

out vec4 FragColor;

void main()
{
	vec3 albedo = texture(material.diffuse, texCoord).rgb;
	FragColor = vec4(lightingColor * albedo, 1.0);
}