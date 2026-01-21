#include "Light.h"

// static definitions
unsigned int Light::VAO = 0;
unsigned int Light::VBO = 0;
unsigned int Light::EBO = 0;
int Light::nextID = 0;
bool Light::initialized = false;

// abstract Light class
// --------------------

Light::Light(unsigned int& depthMapFBO) : ID(nextID++) {
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

	initBuffers();

	// depth map
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f,1.0f,1.0f,1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
		// positions		// normal vectors		// texture coords
		// front
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	0.0f, 1.0f,

		// back
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 1.0f,

		 // left
		 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		 -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,

		 // right
		 0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,

		 // top
		 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
		 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 1.0f,

		 // bottom
		 -0.5f,  -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
		  0.5f,  -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
		  0.5f,  -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
		 -0.5f,  -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
	};

	unsigned int indices[] = {
		// front
		0, 1, 2,	2, 3, 0,
		// back
		4, 5, 6,	6, 7, 4,
		// left
		8, 9, 10,	10, 11, 8,
		// right
		12, 13, 14, 14, 15, 12,
		// top
		16, 17, 18,	18, 19, 16,
		// bottom
		20, 21, 22,	22, 23, 20
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

unsigned int Light::getDepthMap() const
{
	return depthMap;
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

float Light::getNearPlane() const
{
	return nearPlane;
}

float Light::getFarPlane() const {
	return farPlane;
}

glm::mat4 Light::getView() const {
	return view;
}

glm::mat4 Light::getProjection() const {
	return projection;
}

glm::mat4 Light::getLightMatrix() const {
	return lightSpaceMatrix;
}

// shadow control

void Light::setCastShadow(const bool enable) {
	castsShadow = enable;
}

bool Light::isShadowCaster() const {
	return castsShadow;
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

void Light::updateDepthShader(Shader*& shader)
{
	shader->use();

	shader->setMat4fv("lightSpaceMatrix", lightSpaceMatrix);
}

// opengl

int Light::getID() const {
	return ID;
}

// Directional Light class
// -----------------------

DirectionalLight::DirectionalLight(unsigned int& depthMapFBO) : Light(depthMapFBO) {
	castsShadow = true;
	
	// transformation
	nearPlane = 1.0f;
	farPlane = 60.0f;
	float orthoSize = 30.0f;
	projection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, nearPlane, farPlane);
	
	distance = 30.0f;
	position = glm::vec3(0.0f, 0.0f, 0.0f) - direction * distance;
	view = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	lightSpaceMatrix = projection * view;
}

DirectionalLight::~DirectionalLight() {}

// position

void DirectionalLight::setPosition(const glm::vec3 _pos) {}

void DirectionalLight::setDirection(const float _pitch, const float _yaw) {
	Light::setDirection(_pitch, _yaw);

	position = glm::vec3(0.0f, 0.0f, 0.0f) - direction * distance;
	view = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = projection * view;
}

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

void DirectionalLight::updateObjectShader(Shader*& objectShader, unsigned int typeCount) const {
	objectShader->use();

	objectShader->setVec3fv("dirLight[" + std::to_string(typeCount) + "].direction", direction);
	objectShader->setVec3fv("dirLight[" + std::to_string(typeCount) + "].color", color);

	objectShader->setVec3fv("dirLight[" + std::to_string(typeCount) + "].ambient", ambient);
	objectShader->setVec3fv("dirLight[" + std::to_string(typeCount) + "].diffuse", diffuse);
	objectShader->setVec3fv("dirLight[" + std::to_string(typeCount) + "].specular", specular);
}

// Point Light class
// -----------------

PointLight::PointLight(unsigned int& depthMapFBO) : Light(depthMapFBO) {
	castsShadow = false;
	
	constant = 1.0f;
	linear = 0.045f;
	quadratic = 0.0075f;

	nearPlane = 0.1f;
	farPlane = 30.0f;
	projection = glm::perspective(glm::radians(90.0f), 1.0f, nearPlane, farPlane);

	view = glm::lookAt(position, position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	//glm::mat4 view0 = glm::lookAt(position, position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	//glm::mat4 view1 = glm::lookAt(position, position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	//glm::mat4 view2 = glm::lookAt(position, position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//glm::mat4 view3 = glm::lookAt(position, position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//glm::mat4 view4 = glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	//glm::mat4 view5 = glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	lightSpaceMatrix = projection * view;
}

PointLight::~PointLight() {}

void PointLight::setDirection(const float _pitch, const float _yaw) {};

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

	shader.use();

	shader.setMat4fv("model", model);
	shader.setVec3fv("lightColor", color);

	glBindVertexArray(getVAO());
	glDrawElements(GL_TRIANGLES, getVertexCount(), GL_UNSIGNED_INT, 0);
}

void PointLight::updateObjectShader(Shader*& objectShader, unsigned int typeCount) const {
	objectShader->use();

	objectShader->setVec3fv("pointLight[" + std::to_string(typeCount) + "].position", position);

	objectShader->setFloat("pointLight[" + std::to_string(typeCount) + "].constant", constant);
	objectShader->setFloat("pointLight[" + std::to_string(typeCount) + "].linear", linear);
	objectShader->setFloat("pointLight[" + std::to_string(typeCount) + "].quadratic", quadratic);

	objectShader->setVec3fv("pointLight[" + std::to_string(typeCount) + "].color", color);
	objectShader->setVec3fv("pointLight[" + std::to_string(typeCount) + "].ambient", ambient);
	objectShader->setVec3fv("pointLight[" + std::to_string(typeCount) + "].diffuse", diffuse);
	objectShader->setVec3fv("pointLight[" + std::to_string(typeCount) + "].specular", specular);
}

// Spot Light class
// ----------------

SpotLight::SpotLight(unsigned int& depthMapFBO) : PointLight(depthMapFBO){
	castsShadow = true;
	
	innerCutOff = 12.5f;
	outerCutOff = 17.5f;
	view = glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));

	nearPlane = 0.3f;
	farPlane = 10.0f;
	projection = glm::perspective(glm::radians(outerCutOff * 2), 1.0f, nearPlane, farPlane);

	lightSpaceMatrix = projection * view;
}

SpotLight::~SpotLight() {}

void SpotLight::setPosition(const glm::vec3 _pos) {
	Light::setPosition(_pos);
	view = glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = projection * view;
}

void SpotLight::setDirection(const float _pitch, const float _yaw) {
	Light::setDirection(_pitch, _yaw);
	view = glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = projection * view;
}

void SpotLight::setDirection(const glm::vec3 _direction) {
	Light::setDirection(_direction);
	view = glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = projection * view;
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
	projection = glm::perspective(glm::radians(outerCutOff * 2), 1.0f, nearPlane, farPlane);
	lightSpaceMatrix = projection * view;
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

	shader.use();

	shader.setMat4fv("model", model);
	shader.setVec3fv("lightColor", color);

	glBindVertexArray(getVAO());
	glDrawElements(GL_TRIANGLES, getVertexCount(), GL_UNSIGNED_INT, 0);
}

void SpotLight::updateObjectShader(Shader*& objectShader, unsigned int typeCount) const {
	objectShader->use();

	objectShader->setVec3fv("spotLight[" + std::to_string(typeCount) + "].position", position);
	objectShader->setVec3fv("spotLight[" + std::to_string(typeCount) + "].direction", direction);

	objectShader->setFloat("spotLight[" + std::to_string(typeCount) + "].constant", constant);
	objectShader->setFloat("spotLight[" + std::to_string(typeCount) + "].linear", linear);
	objectShader->setFloat("spotLight[" + std::to_string(typeCount) + "].quadratic", quadratic);

	objectShader->setFloat("spotLight[" + std::to_string(typeCount) + "].innerCutOff", glm::cos(glm::radians(innerCutOff)));
	objectShader->setFloat("spotLight[" + std::to_string(typeCount) + "].outerCutOff", glm::cos(glm::radians(outerCutOff)));

	objectShader->setVec3fv("spotLight[" + std::to_string(typeCount) + "].color", color);
	objectShader->setVec3fv("spotLight[" + std::to_string(typeCount) + "].ambient", ambient);
	objectShader->setVec3fv("spotLight[" + std::to_string(typeCount) + "].diffuse", diffuse);
	objectShader->setVec3fv("spotLight[" + std::to_string(typeCount) + "].specular", specular);
}