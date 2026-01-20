#include "Light.h"

// static definitions
unsigned int Light::VAO = 0;
unsigned int Light::VBO = 0;
int Light::nextID = 0;
bool Light::initialized = false;

// abstract Light class
// --------------------

Light::Light() : ID(nextID++) {
	// color
	color = glm::vec3(1.0f);
	storedColor = color; // Initialize storedColor

	ambient = color * 0.1f;
	diffuse = color;
	specular = color;

	// position and direction
	position = glm::vec3(0.0f);
	direction = glm::vec3(0.0f, 0.0f, -1.0f);
	baseDir = direction;
	yaw = 270;
	pitch = 0;

	// orbital motion
	orbitalMotion = false;
	orbitCenter = position;
	orbitAngle = 0.0f; // degree
	rotationalFreq = 45.0f; // degree
	radius = 0.8f;

	enabled = true;      // Lights are enabled by default
}

Light::~Light() {}

// color

void Light::setColor(const glm::vec3 _color) {
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

// position and direction

void Light::setPosition(const glm::vec3 _pos) {
	position = _pos;
	orbitCenter = _pos;
}

void Light::setDirection(const float _pitch, const float _yaw) {
	setPitch(_pitch);
	setYaw(_yaw);

	float x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
	float y = glm::sin(glm::radians(pitch));
	float z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));

	direction = glm::normalize(glm::vec3(x, y, z));
	baseDir = direction;
}

void Light::setDirection(const glm::vec3 _direction) {
	direction = glm::normalize(_direction);
	baseDir = direction;
}

// angle in degree
void Light::setYaw(const int _angle) {
	yaw = _angle;
}

// angle in degree
void Light::setPitch(const int _angle) {
	pitch = _angle;
}

// orbital motion

// enable/disable orbital motion
void Light::setOrbital(const bool _orbitalMotion) {
	orbitalMotion = _orbitalMotion;
}

// orbital motion radius
void Light::setRadius(float _radius) {
	radius = _radius;
}

// orbital motion speed
void Light::setRotationalFreq(float _freq) {
	rotationalFreq = _freq;
}

void Light::updateOrbitalPosition() {
	if (!orbitalMotion) return;

	position = orbitCenter + 
		glm::vec3(cosf(orbitAngle) * radius, 0.0f, sinf(orbitAngle) * radius);
}

void Light::updateOrbitalDirection() {
	if (!orbitalMotion) return;

	direction = glm::normalize(glm::vec3(cosf(orbitAngle), 0.0f, sinf(orbitAngle)));

	yaw = atan2(direction.z, direction.x) * 180 / glm::pi<float>() + 180.0f;
}

// opengl

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

// visibility

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

// position and direction

glm::vec3 Light::getPosition() const {
	return position;
}

glm::vec3 Light::getDirection() const {
	return direction;
}

float Light::getX() const {
	return position.x;
}

float Light::getY() const {
	return position.y;
}

float Light::getZ() const {
	return position.z;
}

int Light::getYaw() const {
	return yaw;
}

int Light::getPitch() const {
	return pitch;
}

// color

glm::vec3 Light::getColor() const
{
	return color;
}

//glm::vec3& Light::getColor_Ref()
//{
//	return color;
//}

glm::vec3 Light::getStoredColor() const
{
	return storedColor;
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

// orbital motion

bool Light::isOrbital() const
{
	return orbitalMotion;
}

float Light::getRadius() const {
	return radius;
}

float Light::getRotationalFreq() const {
	return rotationalFreq;
}

// opengl

int Light::getID() const {
	return ID;
}

// Directional Light class
// -----------------------

DirectionalLight::DirectionalLight(Shader& _shader) : Light(_shader) {
	//direction = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
}

DirectionalLight::~DirectionalLight() {}

// position

void DirectionalLight::setPosition(const glm::vec3 _pos) {}

float DirectionalLight::getX() const {
	return 0.0f;
}

float DirectionalLight::getY() const {
	return 0.0f;
}

float DirectionalLight::getZ() const {
	return 0.0f;
}

glm::vec3 DirectionalLight::getPosition() const {
	return glm::vec3(0.0f, 0.0f, 0.0f);
}

// opengl

std::string DirectionalLight::getType() const {
	return "Directional";
}

void DirectionalLight::draw(Shader& shader, const float deltaTime) {
	if (!isEnabled()) return;

	if (orbitalMotion) {
		orbitAngle += glm::radians(rotationalFreq) * deltaTime;
		orbitAngle = fmod(orbitAngle, glm::two_pi<float>());

		updateOrbitalDirection();
	}
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

PointLight::PointLight() : Light() {
	constant = 1.0f;
	linear = 0.045f;
	quadratic = 0.0075f;
	//orbitCenter = position;
	//orbitalMotion = false;
}

PointLight::~PointLight() {}

void PointLight::setDirection(const float _yaw, const float _pitch) {};

void PointLight::setDirection(const glm::vec3 _direction) {}

void PointLight::setYaw(const int _angle) {};

void PointLight::setPitch(const int _angle) {};

float PointLight::getConstant() const {
	return constant;
}

float PointLight::getLinear() const {
	return linear;
}

float PointLight::getQuadratic() const {
	return quadratic;
}

glm::vec3 PointLight::getDirection() const {
	return glm::vec3(0.0f, 0.0f, 0.0f);
}

std::string PointLight::getType() const {
	return "Point";
}

void PointLight::draw(Shader& shader, const float deltaTime) {
	if (!isEnabled()) return;

	if (orbitalMotion) {
		orbitAngle += glm::radians(rotationalFreq) * deltaTime;
		orbitAngle = fmod(orbitAngle, glm::two_pi<float>());

		updateOrbitalPosition();
	}

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(0.2f));

	shader.setMat4fv("model", model);
	shader.setVec3fv("lightColor", color);

	glBindVertexArray(getVAO());
	glDrawArrays(GL_TRIANGLES, 0, getVertexCount());
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

SpotLight::SpotLight() : PointLight(){
	innerCutOff = 12.5f;
	outerCutOff = 17.5f;
}

SpotLight::~SpotLight() {}

void SpotLight::setDirection(const float _yaw, const float _pitch) {
	Light::setDirection(_yaw, _pitch);
}

void SpotLight::setDirection(const glm::vec3 _direction) {
	Light::setDirection(_direction);
}

void SpotLight::setYaw(const int _angle) {
	Light::setYaw(_angle);
}

void SpotLight::setPitch(const int _angle) {
	Light::setPitch(_angle);
}

void SpotLight::setInnerCutOff(const float _inner) {
	innerCutOff = _inner;
}

void SpotLight::setOuterCutOff(const float _outer) {
	outerCutOff = _outer;
}

glm::vec3 SpotLight::getDirection() const {
	return Light::getDirection();
}

float SpotLight::getInnerCutOff() const {
	return innerCutOff;
}

float SpotLight::getOuterCutOff() const {
	return outerCutOff;
}

std::string SpotLight::getType() const {
	return "Spot";
}

void SpotLight::updateOrbitalDirection() {
	if (!orbitalMotion) return;

	direction = glm::normalize(orbitCenter - position);

	yaw = atan2(direction.z, direction.x) * 180 / glm::pi<float>() + 180.0f;
}

void SpotLight::draw(Shader& shader, const float deltaTime) {
	if (!isEnabled()) return;

	if (orbitalMotion) {
		orbitAngle += glm::radians(rotationalFreq) * deltaTime;
		orbitAngle = fmod(orbitAngle, glm::two_pi<float>());

		updateOrbitalPosition();
		updateOrbitalDirection();
	}

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(0.2f));

	shader.setMat4fv("model", model);
	shader.setVec3fv("lightColor", color);

	glBindVertexArray(getVAO());
	glDrawArrays(GL_TRIANGLES, 0, getVertexCount());
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