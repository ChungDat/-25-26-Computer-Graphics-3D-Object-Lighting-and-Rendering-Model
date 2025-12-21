#version 330 core

// INPUTS from the Vertex Shader
in vec3 vertexLightColor; // The interpolated light color
in vec2 texCoord;         // The interpolated texture coordinates

// OUTPUT
out vec4 FragColor;

// UNIFORMS
// We need the material samplers here now, just like in the Phong shader
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	// Shininess is not needed here
};

uniform Material material1;

void main()
{
    // 1. Get the base color of the object from the diffuse texture map
	vec3 albedo = texture(material1.diffuse, texCoord).rgb;

    // 2. Modulate (multiply) the light color by the object's texture color
	FragColor = vec4(vertexLightColor * albedo, 1.0);
}