#version 330 core

// in vec3 outColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
in vec3 LightPos;

uniform sampler2D texture1;
uniform vec3 lightColor;
uniform vec3 objectColor;

out vec4 FragColor;

void main() {
	float ambientStrength = 0.2f;
	float specularStrength = 0.8f;
	float shininess = 32;

	// ambient
	vec3 ambient = ambientStrength * lightColor;

	// diffuse	
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(LightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular (use halfway vector) — usually gives broader, visually nicer highlights
    vec3 viewDir = normalize(-fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specFactor = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = specularStrength * specFactor * lightColor;

	// FragColor = texture(texture1, texCoord) * vec4((ambient + diffuse + specular) * objectColor, 1.0f);
	FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0f);

}