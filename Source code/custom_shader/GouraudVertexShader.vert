#version 330 core

layout (location = 0) in vec3 aPos;      // model-space position
layout (location = 1) in vec3 aNormal;   // model-space normal
layout (location = 2) in vec2 aTexCoord;

out vec3 lightingColor;
out vec2 texCoord;

uniform mat4 model; // model -> world
uniform mat4 view; // world -> view
uniform mat4 projection; // view -> clipping

uniform mat4 horizontalRotate;
uniform mat4 verticalRotate;

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

uniform vec3 viewPos; // world-space camera position

uniform DirectionalLight dirLight[NR_DIR_LIGHTS];
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform SpotLight spotLight[NR_SPOT_LIGHTS];

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    mat4 modelTransform = model * verticalRotate * horizontalRotate;
	vec4 worldPos = modelTransform * vec4(aPos, 1.0f);
	gl_Position = projection * view * worldPos;
    texCoord = aTexCoord;

	// world-space position
	vec3 fragPos = vec3(worldPos);

	// world-space normal
    // transform normal with normal matrix (handles non-uniform scale)
	mat3 normalMatrix = mat3(transpose(inverse(modelTransform)));
	vec3 norm = normalize(normalMatrix * aNormal); // World-space normal

	// world-space view direction
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
    
	lightingColor = result;
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	
	// ambient
//	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));
	vec3 ambient = light.ambient;

	// diffuse
	float diff = max(dot(lightDir, normal), 0.0);
//	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
	vec3 diffuse = light.diffuse * diff;

//	// specular
//	vec3 reflectDir = reflect(-lightDir, normal);
//	float specFactor = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
////	vec3 specular = light.specular * specFactor * vec3(texture(material.specular, texCoord));
//	vec3 specular = light.specular * specFactor;

//	return (ambient + diffuse + specular);
	return (ambient + diffuse);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);

	// ambient
	vec3 ambient = light.ambient;

	// diffuse
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * diff;

//	// specular
//	vec3 reflectDir = reflect(-lightDir, normal);
//	float specFactor = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
//	vec3 specular = light.specular * specFactor;

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	
	ambient *= attenuation;
	diffuse *= attenuation;
//	specular *= attenuation;

//	return (ambient + diffuse + specular);
	return (ambient + diffuse);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);

	// ambient
	vec3 ambient = light.ambient;

	// diffuse
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * diff;

//	// specular
//	vec3 reflectDir = reflect(-lightDir, normal);
//	float specFactor = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
//	vec3 specular = light.specular * specFactor;

	// intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light. quadratic * distance * distance);

	ambient *= intensity * attenuation;
	diffuse *= intensity * attenuation;
//	specular *= intensity * attenuation;
	
//	return (ambient + diffuse + specular);
	return (ambient + diffuse);
}
