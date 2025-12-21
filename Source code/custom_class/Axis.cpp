#include "Axis.h"

Axis::Axis(Shader& _shader) : shader(_shader) {
	initialized = false;
	VAO = 0;
	VBO = 0;
	enabled = true;
}

void Axis::initBuffers() {
	if (initialized) return;
	float vertices[] = {
		// position				// color
		// X-Axis (Red)
		-100.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		100.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		// Y-Axis (Green)
		0.0f, -100.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		0.0f, 100.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		// Z-Axis (Blue)
		0.0f, 0.0f, -100.0f,		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 100.0f,		0.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	initialized = true;
}

void Axis::draw(const glm::mat4& view, const glm::mat4& projection) {
	shader.use();
	shader.setMat4fv("view", view);
	shader.setMat4fv("projection", projection);

	glm::mat4 axisModel = glm::mat4(1.0f);
	shader.setMat4fv("model", axisModel);

	// Make the lines thicker for better visibility
	glLineWidth(3.0f);
	glBindVertexArray(getVAO());
	glDrawArrays(GL_LINES, 0, 6); // We are drawing 6 vertices as lines
	glBindVertexArray(0);

	// Reset line width to default
	glLineWidth(1.0f);
}

unsigned int Axis::getVAO() {
	initBuffers();
	return VAO;
}

void Axis::enable() {
	enabled = true;
}

void Axis::disable() {
	enabled = false;
}

bool Axis::isEnabled() {
	return enabled;
}