#version 330 core

layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 horizontalRotate;
uniform mat4 verticalRotate;

void main() {
    mat4 modelTransform = model * verticalRotate * horizontalRotate;

	vec4 worldPos = modelTransform * vec4(aPos, 1.0f);
	vec4 viewPos = view * worldPos;
	gl_Position = projection * viewPos;

	fragPos = vec3(viewPos);

    // transform normal with normal matrix (handles non-uniform scale)
	mat3 normalMatrix = mat3(transpose(inverse(view * modelTransform)));
	normal = normalMatrix * aNormal;

	texCoord = aTexCoord;
}