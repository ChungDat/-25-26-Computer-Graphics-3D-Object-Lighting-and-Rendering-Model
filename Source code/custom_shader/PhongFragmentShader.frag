#version 330 core

struct Material {
	// ambient is usually set to the same as diffuse
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;

	float shininess;
};

struct DirectionalLight {
	vec3 direction; // world-space direction

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position; // world-space position
	
	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position; // world-space position
	vec3 direction; // world-space direction

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float innerCutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;
};

#define NR_DIR_LIGHTS 1
#define NR_POINT_LIGHTS 5
#define NR_SPOT_LIGHTS 1

in vec2 texCoord;
in vec3 fragNormal; // world-space normal
in vec3 fragPos; // world-space position

uniform Material material;
uniform vec3 viewPos; // world-space camera position

uniform DirectionalLight dirLight[NR_DIR_LIGHTS];
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform SpotLight spotLight[NR_SPOT_LIGHTS];

out vec4 FragColor;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);

void main() {
	vec3 norm = normalize(fragNormal);
	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 result = vec3(0.0);

	for (int i = 0; i < NR_DIR_LIGHTS; i++) {
		result += CalcDirLight(dirLight[i], norm, viewDir);
	}
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += CalcPointLight(pointLight[i], norm, fragPos, viewDir);
	}
	for (int i = 0; i < NR_SPOT_LIGHTS; i++) {
		result += CalcSpotLight(spotLight[i], norm, viewDir);
	}

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));

	// diffuse
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));

	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float specFactor = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = light.specular * specFactor * vec3(texture(material.specular, texCoord));

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	
	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));

	// diffuse
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));

	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float specFactor = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = light.specular * specFactor * vec3(texture(material.specular, texCoord));

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light. quadratic * distance * distance);
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);

	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));

	// diffuse
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));

	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float specFactor = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = light.specular * specFactor * vec3(texture(material.specular, texCoord));

	// intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light. quadratic * distance * distance);

	ambient *= intensity * attenuation;
	diffuse *= intensity * attenuation;
	specular *= intensity * attenuation;
	
	return (ambient + diffuse + specular);
}