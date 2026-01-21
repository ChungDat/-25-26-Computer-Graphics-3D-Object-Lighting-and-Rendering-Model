#include "Axis.h"

Axis::Axis(Shader& _shader) : shader(_shader) {
	initialized = false;
	VAO = 0;
	VBO = 0;
	enabled = true;
}

Axis::~Axis() {
	if (initialized) {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
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

Grid::~Grid() {
    if (initialized) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
}

void Grid::initBuffers(float size = 100.0f, int divisions = 100) {
    if (initialized) return;

    // Calculate number of lines (divisions + 1 lines in each direction)
    int linesPerAxis = divisions + 1;
    int totalLines = linesPerAxis * 2; // for both X and Z axes
    vertexCount = totalLines * 2; // 2 vertices per line

    std::vector<float> vertices;
    vertices.reserve(vertexCount * 6); // 6 floats per vertex (3 pos + 3 color)

    float step = (2.0f * size) / divisions;
    float halfSize = size;

    // Grid color (light gray)
    float r = 0.5f, g = 0.5f, b = 0.5f;

    // Lines parallel to X-axis (running along Z)
    for (int i = 0; i <= divisions; i++) {
        float z = -halfSize + i * step;

        // Highlight center lines with different color
        if (i == divisions / 2) {
            r = 0.7f; g = 0.7f; b = 0.7f;
        }
        else {
            r = 0.5f; g = 0.5f; b = 0.5f;
        }

        vertices.push_back(-halfSize); vertices.push_back(0.0f); vertices.push_back(z);
        vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);

        vertices.push_back(halfSize); vertices.push_back(0.0f); vertices.push_back(z);
        vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);
    }

    // Lines parallel to Z-axis (running along X)
    for (int i = 0; i <= divisions; i++) {
        float x = -halfSize + i * step;

        // Highlight center lines with different color
        if (i == divisions / 2) {
            r = 0.7f; g = 0.7f; b = 0.7f;
        }
        else {
            r = 0.5f; g = 0.5f; b = 0.5f;
        }

        vertices.push_back(x); vertices.push_back(0.0f); vertices.push_back(-halfSize);
        vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);

        vertices.push_back(x); vertices.push_back(0.0f); vertices.push_back(halfSize);
        vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

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

void Grid::draw(const glm::mat4& view, const glm::mat4& projection) {
    shader.use();
    shader.setMat4fv("view", view);
    shader.setMat4fv("projection", projection);

    glm::mat4 gridModel = glm::mat4(1.0f);
    shader.setMat4fv("model", gridModel);

    // Make the lines slightly thicker for better visibility
    glLineWidth(1.5f);

    glBindVertexArray(getVAO());
    glDrawArrays(GL_LINES, 0, vertexCount);
    glBindVertexArray(0);

    // Reset line width to default
    glLineWidth(1.0f);
}

unsigned int Grid::getVAO() {
    initBuffers();
    return VAO;
}