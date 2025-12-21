#version 330 core

// INPUTS
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// OUTPUTS
out vec3 vertexLightColor; // The calculated color of the light at this vertex
out vec2 texCoord;         // Pass through texture coordinates for the fragment shader

// UNIFORMS (Same as before)
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 horizontalRotate;
uniform mat4 verticalRotate;

// --- Lighting Uniforms ---
struct Material {
	// We don't need the samplers here anymore, but we need shininess
	float shininess;
};

struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	float constant;
	float linear;
	float quadratic;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float innerCutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define NR_DIR_LIGHTS 1
#define NR_POINT_LIGHTS 5
#define NR_SPOT_LIGHTS 1

uniform vec3 viewPos;
uniform Material material2;
uniform DirectionalLight dirLight[NR_DIR_LIGHTS];
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform SpotLight spotLight[NR_SPOT_LIGHTS];


// --- Helper Lighting Functions (Modified to remove texture lookups) ---

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	vec3 ambient = light.ambient; // REMOVED: texture lookup
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * diff; // REMOVED: texture lookup
	vec3 reflectDir = reflect(-lightDir, normal);
	float specFactor = pow(max(dot(reflectDir, viewDir), 0.0), material2.shininess);
	vec3 specular = light.specular * specFactor; // REMOVED: texture lookup
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 ambient = light.ambient;
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * diff;
	vec3 reflectDir = reflect(-lightDir, normal);
	float specFactor = pow(max(dot(reflectDir, viewDir), 0.0), material2.shininess);
	vec3 specular = light.specular * specFactor;
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	return CalcPointLight(PointLight(light.position, light.constant, light.linear, light.quadratic, light.ambient, light.diffuse, light.specular), normal, fragPos, viewDir) * intensity;
}


void main() {
    // Standard Transformations
    mat4 modelTransform = model * verticalRotate * horizontalRotate;
	vec4 worldPos = modelTransform * vec4(aPos, 1.0f);
	gl_Position = projection * view * worldPos;
    texCoord = aTexCoord; // Pass through for the fragment shader

    // Gouraud Lighting Calculations
	vec3 fragPos = vec3(worldPos);
	mat3 normalMatrix = mat3(transpose(inverse(modelTransform)));
	vec3 norm = normalize(normalMatrix * aNormal);
	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 result = vec3(0.0);

	for (int i = 0; i < NR_DIR_LIGHTS; i++) {
		result += CalcDirLight(dirLight[i], norm, viewDir);
	}
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += CalcPointLight(pointLight[i], norm, fragPos, viewDir);
	}
	for (int i = 0; i < NR_SPOT_LIGHTS; i++) {
		result += CalcSpotLight(spotLight[i], norm, fragPos, viewDir);
	}
    
	vertexLightColor = result;
}