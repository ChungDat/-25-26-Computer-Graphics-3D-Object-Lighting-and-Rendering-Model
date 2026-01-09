#version 330 core

struct Material {
	vec3 diffuse;
};

in vec3 lightingColor;
in vec2 texCoord;

out vec4 FragColor;

uniform Material material;
uniform sampler2D diffuseMap;
uniform sampler2D emissionMap;

uniform bool useDiffuseMap;
uniform bool useEmissionMap;

void main()
{
	vec3 albedo = useDiffuseMap ? texture(diffuseMap, texCoord).rgb : material.diffuse;
	FragColor = vec4(lightingColor * albedo, 1.0);
}