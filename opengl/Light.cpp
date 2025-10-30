#include "Light.h"

// static definitions
unsigned int Light::VAO = 0;
unsigned int Light::VBO = 0;
bool Light::initialized = false;

// abstract Light class
// --------------------

Light::Light(Shader& shader) : shader(shader) {
	color = glm::vec3(1.0f);
	storedColor = color; // Initialize storedColor
	enabled = true;      // Lights are enabled by default
	ambient = color * 0.1f;
	diffuse = color;
	specular = color;
}

void Light::setColor(const glm::vec3& _color) {
	color = _color;
	ambient = color * 0.1f;
	diffuse = color;
	specular = color;
}

void Light::initBuffers() {
	if (initialized) return;
	float vertices[] = {
		// positions          // normal vectors		// texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	initialized = true;

}

unsigned int Light::getVAO() const {
	initBuffers();
	return VAO;
}

unsigned int Light::getVertexCount() const {
	return 36; // 6 faces * 2 triangles * 3 vertices
}

glm::vec3 Light::getAmbient() const {
	return ambient;
}

glm::vec3 Light::getDiffuse() const {
	return diffuse;
}

glm::vec3 Light::getSpecular() const {
	return specular;
}

void Light::enable() {
	if (!enabled && color != glm::vec3(0.0f)) {
		setColor(storedColor);
		enabled = true;
	}
}

void Light::disable() {
	if (enabled) {
		storedColor = color; // Save the current color
		setColor(glm::vec3(0.0f)); // Set color to black
		enabled = false;
	}
}

bool Light::isEnabled() const {
	return enabled;
}

Light::~Light() {}

// Directional Light class
// -----------------------

DirectionalLight::DirectionalLight(Shader& shader) : Light(shader) {
	direction = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
}

void DirectionalLight::setDirection(const glm::vec3& _dir) {
	direction = _dir;
}

glm::vec3 DirectionalLight::getDirection() const {
	return direction;
}

void DirectionalLight::updateObjectShader(Shader& objectShader, unsigned int typeCount) const {
	objectShader.use();
	objectShader.setVec3fv("dirLight[" + std::to_string(typeCount) + "].direction", direction);
	objectShader.setVec3fv("dirLight[" + std::to_string(typeCount) + "].ambient", ambient);
	objectShader.setVec3fv("dirLight[" + std::to_string(typeCount) + "].diffuse", diffuse);
	objectShader.setVec3fv("dirLight[" + std::to_string(typeCount) + "].specular", specular);
}

// Point Light class
// -----------------

PointLight::PointLight(Shader& shader) : Light(shader) {
	position = glm::vec3(0.0f);
	constant = 1.0f;
	linear = 0.045f;
	quadratic = 0.0075f;
	orbitCenter = position;
	circularMotion = false;
}

void PointLight::draw(const glm::mat4& view, const glm::mat4& projection) const {
	shader.use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(0.2f));
	shader.setMat4fv("model", model);
	shader.setVec3fv("lightColor", color);
	shader.setMat4fv("view", view);
	shader.setMat4fv("projection", projection);

	glBindVertexArray(getVAO());
	glDrawArrays(GL_TRIANGLES, 0, getVertexCount());
}

void PointLight::update(float time) {
	if (!circularMotion) return;

	// compute angle in radians
	float angle = time * glm::radians(rotationalFreq);

	// orbit in XZ plane around orbitCenter
	position.x = orbitCenter.x + cosf(angle) * radius;
	position.y = orbitCenter.y; // keep same Y as center
	position.z = orbitCenter.z + sinf(angle) * radius;
}

void PointLight::setPosition(const glm::vec3& _pos) {
	position = _pos;
	orbitCenter = _pos;
}

void PointLight::setCircularMotion(bool _circularMotion) {
	circularMotion = _circularMotion;
}

void PointLight::setRadius(float _radius) {
	radius = _radius;
}

void PointLight::setRotationalFreq(float _freq) {
	rotationalFreq = _freq;
}

float PointLight::getConstant() const {
	return constant;
}

float PointLight::getLinear() const {
	return linear;
}

float PointLight::getQuadratic() const {
	return quadratic;
}

void PointLight::updateObjectShader(Shader& objectShader, unsigned int typeCount) const {
	objectShader.use();
	objectShader.setVec3fv("pointLight[" + std::to_string(typeCount) + "].position", position);
	objectShader.setVec3fv("pointLight[" + std::to_string(typeCount) + "].ambient", ambient);
	objectShader.setVec3fv("pointLight[" + std::to_string(typeCount) + "].diffuse", diffuse);
	objectShader.setVec3fv("pointLight[" + std::to_string(typeCount) + "].specular", specular);
	objectShader.setFloat("pointLight[" + std::to_string(typeCount) + "].constant", constant);
	objectShader.setFloat("pointLight[" + std::to_string(typeCount) + "].linear", linear);
	objectShader.setFloat("pointLight[" + std::to_string(typeCount) + "].quadratic", quadratic);
}

// Spot Light class
// ----------------

SpotLight::SpotLight(Shader& shader) : PointLight(shader) {
	direction = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
	innerCutOff = 12.5f;
	outerCutOff = 17.5f;
}

void SpotLight::setDirection(const glm::vec3& _dir) {
	direction = glm::normalize(_dir);
}

void SpotLight::setInnerCutOff(float _inner) {
	innerCutOff = _inner;
}

void SpotLight::setOuterCutOff(float _outer) {
	outerCutOff = _outer;
}

float SpotLight::getInnerCutOff() const {
	return innerCutOff;
}

float SpotLight::getOuterCutOff() const {
	return outerCutOff;
}

void SpotLight::updateObjectShader(Shader& objectShader, unsigned int typeCount) const {
	objectShader.use();
	objectShader.setVec3fv("spotLight[" + std::to_string(typeCount) + "].position", position);
	objectShader.setVec3fv("spotLight[" + std::to_string(typeCount) + "].direction", direction);
	objectShader.setVec3fv("spotLight[" + std::to_string(typeCount) + "].ambient", ambient);
	objectShader.setVec3fv("spotLight[" + std::to_string(typeCount) + "].diffuse", diffuse);
	objectShader.setVec3fv("spotLight[" + std::to_string(typeCount) + "].specular", specular);
	objectShader.setFloat("spotLight[" + std::to_string(typeCount) + "].constant", constant);
	objectShader.setFloat("spotLight[" + std::to_string(typeCount) + "].linear", linear);
	objectShader.setFloat("spotLight[" + std::to_string(typeCount) + "].quadratic", quadratic);
	objectShader.setFloat("spotLight[" + std::to_string(typeCount) + "].innerCutOff", glm::cos(glm::radians(innerCutOff)));
	objectShader.setFloat("spotLight[" + std::to_string(typeCount) + "].outerCutOff", glm::cos(glm::radians(outerCutOff)));
}