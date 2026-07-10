#version 330 core

struct Material {
	vec3 diffuse;
};

in vec3 lightingColor;
in vec2 texCoord;
in vec3 fragPos;

out vec4 FragColor;

uniform Material material;
uniform sampler2D diffuseMap;
uniform sampler2D emissionMap;

uniform bool useDiffuseMap;
uniform bool useEmissionMap;

#define NR_DIR_LIGHTS 4
#define NR_POINT_LIGHTS 8
#define NR_SPOT_LIGHTS 4

uniform int numDirLights;
uniform int numPointLights;
uniform int numSpotLights;

uniform sampler2D dirShadowMap[NR_DIR_LIGHTS];
uniform mat4 dirLightSpace[NR_DIR_LIGHTS];
uniform bool dirShadowCast[NR_DIR_LIGHTS];

uniform sampler2D spotShadowMap[NR_SPOT_LIGHTS];
uniform mat4 spotLightSpace[NR_SPOT_LIGHTS];
uniform bool spotShadowCast[NR_SPOT_LIGHTS];

uniform float shadowBias;
uniform float shadowSize;

float ShadowCalculation(sampler2D shadowMap, vec4 fragPosLightSpace);

void main()
{
	vec3 albedo = useDiffuseMap ? texture(diffuseMap, texCoord).rgb : material.diffuse;
	
    float shadow = 0.0;
    int count = 0;

    for (int i = 0; i < numDirLights; i++) {
        if (dirShadowCast[i]) {
            vec4 fragPosLightSpace = dirLightSpace[i] * vec4(fragPos, 1.0);
            shadow += ShadowCalculation(dirShadowMap[i], fragPosLightSpace);
            count++;
        }
    }

    for (int i = 0; i < NR_SPOT_LIGHTS; ++i) {
		if (spotShadowCast[i]) {
            vec4 fragPosLightSpace = spotLightSpace[i] * vec4(fragPos, 1.0);
			shadow += ShadowCalculation(spotShadowMap[i], fragPosLightSpace);
			count++;
		}
	}

    float shadowFactor = 0.0;
    if (count > 0) {
        shadowFactor = clamp(shadow / float(count), 0.0, 1.0);
    }

    vec3 lit = lightingColor * (1.0 - shadowFactor);

    FragColor = vec4(lit * albedo, 1.0);
}

float ShadowCalculation(sampler2D shadowMap, vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
        return 0.0;

    float closestDepth = texture(shadowMap, projCoords.xy).r;

    float currentDepth = projCoords.z;

    // float shadow = currentDepth - shadowBias > closestDepth ? 1.0 : 0.0;

	// PCF
    float shadow = 0.0;
    float texelSize = 1.0 / shadowSize;
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
    	    shadow += currentDepth - shadowBias > texture(shadowMap, projCoords.xy + vec2(x,y) * texelSize).r ? 1.0 : 0.0;
	    }
    }
    shadow /= 9.0;

	return shadow;
}