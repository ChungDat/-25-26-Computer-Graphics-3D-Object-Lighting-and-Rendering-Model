#version 330 core

struct DirectionalLight {
	vec3 direction; // world-space direction
	vec3 color;
};

struct PointLight {
	vec3 position; // world-space position
	vec3 color;
	
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position; // world-space position
	vec3 direction; // world-space direction
	vec3 color;

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

out vec4 FragColor;

uniform vec3 viewPos; // world-space camera position
uniform DirectionalLight dirLight[NR_DIR_LIGHTS];
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform SpotLight spotLight[NR_SPOT_LIGHTS];
uniform vec3 albedo;
uniform float roughness;
uniform float metallic;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 F0, float roughness, float metallic);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 F0, float roughness, float metallic);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 F0, float roughness, float metallic);

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);

void main() {
	vec3 N = normalize(fragNormal);
	vec3 V = normalize(viewPos - fragPos);

	vec3 result = vec3(0.0);

	vec3 F0 = mix(vec3(0.04), albedo, metallic);

	for (int i = 0; i < NR_DIR_LIGHTS; i++) {
		result += CalcDirLight(dirLight[i], N, V, F0, roughness, metallic);
	}
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += CalcPointLight(pointLight[i], N, fragPos, V, F0, roughness, metallic);
	}
	for (int i = 0; i < NR_SPOT_LIGHTS; i++) {
		result += CalcSpotLight(spotLight[i], N, V, F0, roughness, metallic);
	}

	FragColor = vec4(result, 1.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = 3.14159265 * denom * denom;

	return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);

	float ggx1 = GeometrySchlickGGX(NdotV, roughness);
	float ggx2 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 F0, float roughness, float metallic) {
	vec3 lightDir = normalize(-light.direction);
	vec3 H = normalize(lightDir + viewDir);

	float D = DistributionGGX(normal, H, roughness);
	float G = GeometrySmith(normal, viewDir, lightDir, roughness);
	vec3 F = FresnelSchlick(max(dot(H, viewDir), 0.0), F0);

	vec3 num = D * G * F;
	float denom = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.001;
	vec3 specular = num / denom;
	
	vec3 kS = F;
	vec3 kD = (1.0 - kS) * (1.0 - metallic);

	vec3 diffuse = kD * albedo / 3.14159265;

	float NdotL = max(dot(normal, lightDir), 0.0);
	
	return (diffuse + specular) * light.color * NdotL;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 F0, float roughness, float metallic) {
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 H = normalize(lightDir + viewDir);

	float D = DistributionGGX(normal, H, roughness);
	float G = GeometrySmith(normal, viewDir, lightDir, roughness);
	vec3 F = FresnelSchlick(max(dot(H, viewDir), 0.0), F0);

	vec3 num = D * G * F;
	float denom = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.001;
	vec3 specular = num / denom;
	
	vec3 kS = F;
	vec3 kD = (1.0 - kS) * (1.0 - metallic);

	vec3 diffuse = kD * albedo / 3.14159265;

	float NdotL = max(dot(normal, lightDir), 0.0);
	
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / max(distance * distance, 0.001);

	return (diffuse + specular) * light.color * NdotL * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 F0, float roughness, float metallic) {
	vec3 lightDir = normalize(-light.direction);
	vec3 H = normalize(lightDir + viewDir);

	float D = DistributionGGX(normal, H, roughness);
	float G = GeometrySmith(normal, viewDir, lightDir, roughness);
	vec3 F = FresnelSchlick(max(dot(H, viewDir), 0.0), F0);

	vec3 num = D * G * F;
	float denom = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.001;
	vec3 specular = num / denom;
	
	vec3 kS = F;
	vec3 kD = (1.0 - kS) * (1.0 - metallic);

	vec3 diffuse = kD * albedo / 3.14159265;

	float NdotL = max(dot(normal, lightDir), 0.0);
	
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / max(distance * distance, 0.001);

	// intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	return (diffuse + specular) * light.color * NdotL * attenuation * intensity;
}