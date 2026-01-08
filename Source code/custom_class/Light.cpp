#include "Light.h"

// static definitions
unsigned int Light::VAO = 0;
unsigned int Light::VBO = 0;
int Light::nextID = 0;
bool Light::initialized = false;

// abstract Light class
// --------------------

Light::Light(Shader& shader) : shader(shader), ID(nextID++) {
	color = glm::vec3(1.0f);
	storedColor = color; // Initialize storedColor
	enabled = true;      // Lights are enabled by default

	ambient = color * 0.1f;
	diffuse = color;
	specular = color;
}

void Light::setColor(const glm::vec3& _color) {
	if (isEnabled()) {
		color = _color;
		updateAttribute();
	}
	else {
		storedColor = _color;
	}
}

void Light::updateAttribute() {
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

glm::vec3 Light::getDirection() const {
	return glm::vec3(0.0f);
}

glm::vec3 Light::getPosition() const {
	return glm::vec3(0.0f);
}

float Light::getX() const {
	return 0.0f;
}

float Light::getY() const {
	return 0.0f;
}

float Light::getZ() const {
	return 0.0f;
}

glm::vec3 Light::getColor() const
{
	return color;
}

glm::vec3& Light::getColor_Ref()
{
	return this->color;
}

glm::vec3 Light::getStoredColor() const
{
	return storedColor;
}

float Light::getRadius() const {
	return 0.0f;
}

float Light::getInnerCutOff() const {
	return 0.0f;
}

float Light::getOuterCutOff() const {
	return 0.0f;
}

int Light::getID() const {
	return ID;
}

void Light::enable() {
	if (!enabled) {
		enabled = true;
		setColor(storedColor);
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

std::string DirectionalLight::getType() const {
	return "Directional";
}

void DirectionalLight::updateObjectShader(Shader& objectShader, unsigned int typeCount) const {
	objectShader.use();
	objectShader.setVec3fv("dirLight[" + std::to_string(typeCount) + "].direction", direction);

	objectShader.setVec3fv("dirLight[" + std::to_string(typeCount) + "].color", color);
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
	orbitalMotion = false;
}

void PointLight::draw(const glm::mat4& view, const glm::mat4& projection) const {
	if (!isEnabled()) return;
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

void PointLight::updatePosition(float time) {
	if (!orbitalMotion) return;

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
	orbitalMotion = _circularMotion;
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

glm::vec3 PointLight::getPosition() const
{
	return position;
}

float PointLight::getX() const {
	return position.x;
}

float PointLight::getY() const {
	return position.y;
}

float PointLight::getZ() const {
	return position.z;
}

float PointLight::getRadius() const
{
	return radius;
}

std::string PointLight::getType() const {
	return "Point";
}

void PointLight::updateObjectShader(Shader& objectShader, unsigned int typeCount) const {
	objectShader.use();
	objectShader.setVec3fv("pointLight[" + std::to_string(typeCount) + "].position", position);

	objectShader.setFloat("pointLight[" + std::to_string(typeCount) + "].constant", constant);
	objectShader.setFloat("pointLight[" + std::to_string(typeCount) + "].linear", linear);
	objectShader.setFloat("pointLight[" + std::to_string(typeCount) + "].quadratic", quadratic);

	objectShader.setVec3fv("pointLight[" + std::to_string(typeCount) + "].color", color);
	objectShader.setVec3fv("pointLight[" + std::to_string(typeCount) + "].ambient", ambient);
	objectShader.setVec3fv("pointLight[" + std::to_string(typeCount) + "].diffuse", diffuse);
	objectShader.setVec3fv("pointLight[" + std::to_string(typeCount) + "].specular", specular);
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

glm::vec3 SpotLight::getDirection() const
{
	return direction;
}

std::string SpotLight::getType() const {
	return "Spot";
}

void SpotLight::updateObjectShader(Shader& objectShader, unsigned int typeCount) const {
	objectShader.use();
	objectShader.setVec3fv("spotLight[" + std::to_string(typeCount) + "].position", position);
	objectShader.setVec3fv("spotLight[" + std::to_string(typeCount) + "].direction", direction);

	objectShader.setFloat("spotLight[" + std::to_string(typeCount) + "].constant", constant);
	objectShader.setFloat("spotLight[" + std::to_string(typeCount) + "].linear", linear);
	objectShader.setFloat("spotLight[" + std::to_string(typeCount) + "].quadratic", quadratic);

	objectShader.setFloat("spotLight[" + std::to_string(typeCount) + "].innerCutOff", glm::cos(glm::radians(innerCutOff)));
	objectShader.setFloat("spotLight[" + std::to_string(typeCount) + "].outerCutOff", glm::cos(glm::radians(outerCutOff)));

	objectShader.setVec3fv("spotLight[" + std::to_string(typeCount) + "].color", color);
	objectShader.setVec3fv("spotLight[" + std::to_string(typeCount) + "].ambient", ambient);
	objectShader.setVec3fv("spotLight[" + std::to_string(typeCount) + "].diffuse", diffuse);
	objectShader.setVec3fv("spotLight[" + std::to_string(typeCount) + "].specular", specular);
}