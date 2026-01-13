#version 330 core

layout (location = 0) in vec3 aPos; // model-space position
layout (location = 1) in vec3 aNormal; // model-space normal
layout (location = 2) in vec2 aTexCoord; // texture coordinates

out vec2 texCoord;
out vec3 fragNormal; // world-space normal
out vec3 fragPos; // world-space position

uniform mat4 model; // model -> world
uniform mat4 view; // world -> view
uniform mat4 projection; // view -> clipping

void main() {
	// world-space position
	vec4 worldPos = model * vec4(aPos, 1.0f);
	gl_Position = projection * view * worldPos;

	fragPos = vec3(worldPos);

	// world-space normal
    // transform normal with normal matrix (handles non-uniform scale)
	mat3 normalMatrix = mat3(transpose(inverse(model)));
	fragNormal = normalize(normalMatrix * aNormal);
	
	texCoord = aTexCoord;
}