#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

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

in vec2 texCoord;
in vec3 fragNormal; // world-space normal
in vec3 fragPos; // world-space position

out vec4 FragColor;

uniform Material material;
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D emissionMap;

uniform bool useDiffuseMap;
uniform bool useSpecularMap;
uniform bool useEmissionMap;

uniform vec3 viewPos; // world-space camera position

#define NR_DIR_LIGHTS 4
#define NR_POINT_LIGHTS 8
#define NR_SPOT_LIGHTS 4

uniform int numDirLights;
uniform int numPointLights;
uniform int numSpotLights;

uniform DirectionalLight dirLight[NR_DIR_LIGHTS];
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform SpotLight spotLight[NR_SPOT_LIGHTS];

uniform sampler2D dirShadowMap[NR_DIR_LIGHTS];
uniform mat4 dirLightSpace[NR_DIR_LIGHTS];
uniform bool dirShadowCast[NR_DIR_LIGHTS];

uniform sampler2D spotShadowMap[NR_SPOT_LIGHTS];
uniform mat4 spotLightSpace[NR_SPOT_LIGHTS];
uniform bool spotShadowCast[NR_SPOT_LIGHTS];

uniform float shadowBias;
uniform float shadowSize;

vec3 CalcDirLight(DirectionalLight light, sampler2D shadowMap, mat4 lightSpaceMatrix, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, sampler2D shadowMap, mat4 lightSpaceMatrix, vec3 normal, vec3 viewDir);

float ShadowCalculation(sampler2D shadowMap, vec4 fragPosLightSpace);

void main() {
	vec3 norm = normalize(fragNormal);
	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 result = vec3(0.0);

	for (int i = 0; i < numDirLights; i++) {
		result += CalcDirLight(dirLight[i], dirShadowMap[i], dirLightSpace[i], norm, viewDir);
	}
	for (int i = 0; i < numPointLights; i++) {
		result += CalcPointLight(pointLight[i], norm, fragPos, viewDir);
	}
	for (int i = 0; i < numSpotLights; i++) {
		result += CalcSpotLight(spotLight[i], spotShadowMap[i], spotLightSpace[i], norm, viewDir);
	}

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirectionalLight light, sampler2D shadowMap, mat4 lightSpaceMatrix, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

	vec3 ambientColor = useDiffuseMap ? texture(diffuseMap, texCoord).rgb : material.ambient;
	vec3 diffuseColor = useDiffuseMap ? texture(diffuseMap, texCoord).rgb : material.diffuse;
	vec3 specularColor = useSpecularMap ? texture(specularMap, texCoord).rgb : material.specular;

	// ambient
	vec3 ambient = light.ambient * ambientColor;

	// diffuse
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * diff * diffuseColor;

	// specular (use halfway vector) — usually gives broader, visually nicer highlights
    vec3 halfwayDir = normalize(lightDir + viewDir);
	float specFactor = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	vec3 specular = light.specular * specFactor * specularColor;

	// shadow
	vec4 fracPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
	float shadow = ShadowCalculation(shadowMap, fracPosLightSpace);

	vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);

	return lighting;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	
	vec3 ambientColor = useDiffuseMap ? texture(diffuseMap, texCoord).rgb : material.ambient;
	vec3 diffuseColor = useDiffuseMap ? texture(diffuseMap, texCoord).rgb : material.diffuse;
	vec3 specularColor = useSpecularMap ? texture(specularMap, texCoord).rgb : material.specular;

	// ambient
	vec3 ambient = light.ambient * ambientColor;

	// diffuse
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * diff * diffuseColor;

	// specular (use halfway vector) — usually gives broader, visually nicer highlights
    vec3 halfwayDir = normalize(lightDir + viewDir);
	float specFactor = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	vec3 specular = light.specular * specFactor * specularColor;

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light. quadratic * distance * distance);
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, sampler2D shadowMap, mat4 lightSpaceMatrix, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);

	vec3 ambientColor = useDiffuseMap ? texture(diffuseMap, texCoord).rgb : material.ambient;
	vec3 diffuseColor = useDiffuseMap ? texture(diffuseMap, texCoord).rgb : material.diffuse;
	vec3 specularColor = useSpecularMap ? texture(specularMap, texCoord).rgb : material.specular;

	// ambient
	vec3 ambient = light.ambient * ambientColor;

	// diffuse
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * diff * diffuseColor;

	// specular (use halfway vector) — usually gives broader, visually nicer highlights
    vec3 halfwayDir = normalize(lightDir + viewDir);
	float specFactor = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	vec3 specular = light.specular * specFactor * specularColor;

	// intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light. quadratic * distance * distance);

	// shadow
	vec4 fracPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
	float shadow = ShadowCalculation(shadowMap, fracPosLightSpace);

	vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);

	lighting *= intensity * attenuation;
	
	return lighting;
}

float ShadowCalculation(sampler2D shadowMap, vec4 fragPosLightSpace) {
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	// transform to [0,1]
    projCoords = projCoords * 0.5 + 0.5;

	// if outside the depth map bounds, not in shadow
    if(projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
        return 0.0;

    // read depth from depth map
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // current fragment depth from light POV
    float currentDepth = projCoords.z;

    // float shadow = currentDepth - shadowBias > closestDepth ? 1.0 : 0.0;

	// PCF
	float shadow = 0.0;
	float texelSize = 1.0 / shadowSize;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			shadow += currentDepth - shadowBias > texture(shadowMap, projCoords.xy + vec2(x,y) * texelSize).r ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	return shadow;
}