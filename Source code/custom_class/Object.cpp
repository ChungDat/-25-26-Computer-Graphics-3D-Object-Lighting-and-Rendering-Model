#include "Object.h"
#include "../utils.h"

// static definitions

int Object::nextID = 0;

unsigned int Cube::VAO = 0;
unsigned int Cube::VBO = 0;
unsigned int Cube::EBO = 0;
bool Cube::initialized = false;

unsigned int Pyramid::VAO = 0;
unsigned int Pyramid::VBO = 0;
unsigned int Pyramid::EBO = 0;
bool Pyramid::initialized = false;

unsigned int Sphere::VAO = 0;
unsigned int Sphere::VBO = 0;
unsigned int Sphere::EBO = 0;
std::vector<float> Sphere::vertices = std::vector<float>();
std::vector<unsigned int> Sphere::indices = std::vector<unsigned int>();
bool Sphere::initialized = false;

unsigned int Cylinder::VAO = 0;
unsigned int Cylinder::VBO = 0;
unsigned int Cylinder::EBO = 0;
std::vector<float> Cylinder::vertices = std::vector<float>();
std::vector<unsigned int> Cylinder::indices = std::vector<unsigned int>();
bool Cylinder::initialized = false;

// abstract Object class
// ---------------------

Object::Object() : ID(nextID++)
{
	model = glm::mat4(1.0f);
	position = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	rotation = glm::vec3(0.0f);
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;

	material.ambient = glm::vec3(0.1f);
	material.diffuse = glm::vec3(0.5f);
	material.specular = glm::vec3(0.5f);

	//material.shininess = 32.0f;

	material.albedo = glm::vec3(0.9f);
	material.roughness = 0.5f;
	material.metallic = 0.5f;

	texture.diffuseMap = 0;
	texture.specularMap = 0;
	texture.emissionMap = 0;

	texture.albedoMap = 0;
	texture.metallicMap = 0;
	texture.roughnessMap = 0;

	enabled = true;
}

void Object::draw(Shader*& shader) {
	if (!isEnabled()) return;

	setModelMatrix();

	shader->use();

	shader->setMat4fv("model", model);

	shader->setVec3fv("material.ambient", material.ambient);
	shader->setVec3fv("material.diffuse", material.diffuse);
	shader->setVec3fv("material.specular", material.specular);

	//shader->setFloat("material.shininess", material.shininess);

	shader->setVec3fv("material.albedo", material.albedo);
	shader->setFloat("material.metallic", material.metallic);
	shader->setFloat("material.roughness", material.roughness);

	shader->setBool("useDiffuseMap", texture.diffuseMap != 0);
	shader->setBool("useSpecularMap", texture.specularMap != 0);
	shader->setBool("useEmissionMap", texture.emissionMap != 0);

	shader->setBool("useAlbedoMap", texture.albedoMap != 0);
	shader->setBool("useMetallicMap", texture.metallicMap != 0);
	shader->setBool("useRoughnessMap", texture.roughnessMap != 0);

	shader->setInt("diffuseMap", 0);
	shader->setInt("specularMap", 1);
	shader->setInt("emissionMap", 2);

	shader->setInt("albedoMap", 3);
	shader->setInt("metallicMap", 4);
	shader->setInt("roughnessMap", 5);

	glBindVertexArray(getVAO());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture.specularMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture.emissionMap);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture.albedoMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, texture.metallicMap);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texture.roughnessMap);

	glDrawElements(getDrawMode(), getVertexCount(), GL_UNSIGNED_INT, 0);
}

void Object::setDiffusePath(const std::string _diffusePath)
{
	texture.diffusePath = _diffusePath;
	texture.diffuseMap = createTexture(texture.diffusePath.c_str());
}

void Object::setSpecularPath(const std::string _specularPath)
{
	texture.specularPath = _specularPath;
	texture.specularMap = createTexture(texture.specularPath.c_str());
}

void Object::setEmissionPath(const std::string _emissionPath)
{
	texture.emissionPath = _emissionPath;
	texture.emissionMap = createTexture(texture.emissionPath.c_str());
}

void Object::setAlbedoPath(const std::string _albedoPath) {
	texture.albedoPath = _albedoPath;
	texture.albedoMap = createTexture(texture.albedoPath.c_str());
}

void Object::setMetallicPath(const std::string _metallicPath) {
	texture.metallicPath = _metallicPath;
	texture.metallicMap = createTexture(texture.metallicPath.c_str());
}

void Object::setRoughnessPath(const std::string _roughnessPath) {
	texture.roughnessPath = _roughnessPath;
	texture.roughnessMap = createTexture(texture.roughnessPath.c_str());
}

void Object::setAmbient(const glm::vec3 _ambient) {
	material.ambient = _ambient;
}

void Object::setDiffuse(const glm::vec3 _diffuse) {
	material.diffuse = _diffuse;
}

void Object::setSpecular(const glm::vec3 _specular) {
	material.specular = _specular;
}

void Object::setAlbedo(const glm::vec3 _albedo) {
	material.albedo = _albedo;
}

void Object::setRoughness(const float _roughness) {
	material.roughness = _roughness;
}

void Object::setMetallic(const float _metallic) {
	material.metallic = _metallic;
}

void Object::setPosition(const glm::vec3 _pos) {
	position = _pos;
}

void Object::setScale(const glm::vec3 _scale) {
	scale = _scale;
}

void Object::setRotation(const glm::vec3 _rotation) {
	rotation = glm::radians(_rotation);
}

void Object::setRotation(const float _pitch, const float _yaw, const float _roll) {
	setPitch(_pitch);
	setYaw(_yaw);
	setRoll(_roll);

	rotation = glm::radians(glm::vec3(pitch, yaw, roll));
}

void Object::setYaw(const float _yaw) {
	yaw = _yaw;
}

void Object::setPitch(const float _pitch) {
	pitch = _pitch;
}

void Object::setRoll(const float _roll) {
	roll = _roll;
}

void Object::setModelMatrix() {
	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale);
}

void Object::disable() {
	if (enabled)
		enabled = false;
}

void Object::enable() {
	if (!enabled)
		enabled = true;
}

bool Object::isEnabled() const {
	return enabled;
}

glm::vec3 Object::getPosition() const {
	return position;
}

glm::vec3& Object::getPosition_Ref() {
	return this->position;
}

glm::vec3 Object::getScale() const
{
	return scale;
}

glm::vec3 Object::getRotation() const {
	return rotation;
}

float Object::getYaw() const {
	return yaw;
}

float Object::getPitch() const {
	return pitch;
}

float Object::getRoll() const {
	return roll;
}

float Object::getX() const {
	return position.x;
}

float Object::getY() const {
	return position.y;
}

float Object::getZ() const {
	return position.z;
}

glm::vec3 Object::getAmbient() const {
	return material.ambient;
}

glm::vec3 Object::getDiffuse() const {
	return material.diffuse;
}

glm::vec3 Object::getSpecular() const {
	return material.specular;
}

float Object::getShininess() const {
	return material.shininess;
}

glm::vec3 Object::getAlbedo() const {
	return material.albedo;
}

float Object::getRoughness() const {
	return material.roughness;
}

float Object::getMetallic() const {
	return material.metallic;
}

int Object::getID() const {
	return ID;
}

GLenum Object::getDrawMode() const {
	return GL_TRIANGLES;
}

Object::~Object() {}

// Cube class
// ----------

Cube::Cube() : Object()
{
	initBuffers();
}

void Cube::initBuffers() {
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

unsigned int Cube::getVAO() {
	initBuffers();
	return VAO;
}

unsigned int Cube::getVertexCount() const {
	return 36; // 6 faces * 2 triangles * 3 vertices
}

std::string Cube::getType() const
{
	return "Cube";
}

// Pyramid class
// -------------

Pyramid::Pyramid() : Object()
{
	initBuffers();
}

void Pyramid::initBuffers() {
	//			  E (apex)
	//			 / \
	//			/___\
	//	Base:   A----B
	//			|    |
	//			D----C

	if (initialized) return;

	float vertices[] = {
		// positions           // normals             // texcoords
		// Side 1 (C, D, E) front
		-0.5f, 0.0f,  0.5f,   0.0f, 0.707f, 0.707f,    0.0f, 0.0f,  // D
		 0.5f, 0.0f,  0.5f,   0.0f, 0.707f, 0.707f,    1.0f, 0.0f,  // C
		 0.0f, 1.0f,  0.0f,   0.0f, 0.707f, 0.707f,    0.5f, 1.0f,  // E

		// Side 2 (A, B, E) back
		 0.5f, 0.0f, -0.5f,   0.0f, 0.707f, -0.707f,   0.0f, 0.0f,  // B
		-0.5f, 0.0f, -0.5f,   0.0f, 0.707f, -0.707f,   1.0f, 0.0f,  // A
		 0.0f, 1.0f,  0.0f,   0.0f, 0.707f, -0.707f,   0.5f, 1.0f,  // E

		// Side 3 (D, A, E) left
		-0.5f, 0.0f, -0.5f,  -0.707f, 0.707f, 0.0f,    0.0f, 0.0f,  // A
		-0.5f, 0.0f,  0.5f,  -0.707f, 0.707f, 0.0f,    1.0f, 0.0f,  // D
		 0.0f, 1.0f,  0.0f,  -0.707f, 0.707f, 0.0f,    0.5f, 1.0f,  // E

		 // Side 4 (B, C, E) right
		 0.5f, 0.0f,  0.5f,   0.707f, 0.707f, 0.0f,    0.0f, 0.0f,  // C
		 0.5f, 0.0f, -0.5f,   0.707f, 0.707f, 0.0f,    1.0f, 0.0f,  // B
		 0.0f, 1.0f,  0.0f,   0.707f, 0.707f, 0.0f,    0.5f, 1.0f,  // E

		// Base (A, B, C, D) bottom
		-0.5f, 0.0f, -0.5f,   0.0f, -1.0f,  0.0f,      0.0f, 0.0f,  // A
		 0.5f, 0.0f, -0.5f,   0.0f, -1.0f,  0.0f,      1.0f, 0.0f,  // B
		 0.5f, 0.0f,  0.5f,   0.0f, -1.0f,  0.0f,      1.0f, 1.0f,  // C
		-0.5f, 0.0f,  0.5f,   0.0f, -1.0f,  0.0f,      0.0f, 1.0f,  // D
	};

	unsigned int indices[] = {
		// front
		0, 1, 2,
		// back
		3, 4, 5,
		// left
		6, 7, 8,
		// right
		9, 10, 11,
		// bottom
		12, 13, 14,	14, 15, 12
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

unsigned int Pyramid::getVAO() {
	initBuffers();
	return VAO;
}

unsigned int Pyramid::getVertexCount() const {
	return 18; // 4 faces * 1 triangles * 3 vertices + base * 2 triangles * 3 vertices
}

std::string Pyramid::getType() const {
	return "Pyramid";
}

// Sphere class
// ------------

Sphere::Sphere() : Object()
{
	initBuffers();
}

void Sphere::initBuffers() {
	if (initialized) return;

	vertices.clear();

    generateSphere(1.0f, 36, 18, vertices, indices);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

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

unsigned int Sphere::getVAO() {
	initBuffers();
	return VAO;
}

unsigned int Sphere::getVertexCount() const {
	return static_cast<unsigned int>(indices.size());
}

std::string Sphere::getType() const {
	return "Sphere";
}

void Sphere::generateSphere(float radius, unsigned int sectorCount, unsigned int stackCount,
	std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
	const float PI = 3.14159265359f;
	float x, y, z, xy;                            // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;  // normal
	float s, t;                                   // texture coordinates

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;     // from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);          // r * cos(u)
		z = radius * sinf(stackAngle);           // r * sin(u)

		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;        // 0 to 2pi

			// vertex position
			x = xy * cosf(sectorAngle);
			y = xy * sinf(sectorAngle);
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			// normalized vertex normal
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			vertices.push_back(nx);
			vertices.push_back(ny);
			vertices.push_back(nz);

			// texture coordinates
			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			vertices.push_back(s);
			vertices.push_back(t);
		}
	}

	// generate indices
	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);
		k2 = k1 + sectorCount + 1;

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
}

// Cylinder class
// --------------

Cylinder::Cylinder() : Object()
{
	initBuffers();
}

void Cylinder::initBuffers() {
	if (initialized) return;

	vertices.clear();

	generateCylinder(1.0f, 2.0f, 36, vertices, indices);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

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

unsigned int Cylinder::getVAO() {
	initBuffers();
	return VAO;
}

unsigned int Cylinder::getVertexCount() const {
	return static_cast<unsigned int>(indices.size());
}

std::string Cylinder::getType() const {
	return "Cylinder";
}

void Cylinder::generateCylinder(float radius, float height, unsigned int sectorCount,
	std::vector<float>& vertices, std::vector<unsigned int>& indices) {
	const float PI = 3.14159265359f;
	float sectorStep = 2 * PI / sectorCount;
	float sectorAngle;
	float halfHeight = height / 2.0f;

	// ----- SIDE vertices -----
	for (int i = 0; i <= sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;
		float x = cosf(sectorAngle);
		float z = sinf(sectorAngle);

		// position, normal, texcoord (top vertex)
		vertices.push_back(radius * x);
		vertices.push_back(halfHeight);
		vertices.push_back(radius * z);
		vertices.push_back(x); vertices.push_back(0.0f); vertices.push_back(z);
		vertices.push_back((float)i / sectorCount);
		vertices.push_back(1.0f);

		// position, normal, texcoord (bottom vertex)
		vertices.push_back(radius * x);
		vertices.push_back(-halfHeight);
		vertices.push_back(radius * z);
		vertices.push_back(x); vertices.push_back(0.0f); vertices.push_back(z);
		vertices.push_back((float)i / sectorCount);
		vertices.push_back(0.0f);
	}

	// side indices
	for (int i = 0; i < sectorCount; ++i)
	{
		unsigned int k1 = i * 2;
		unsigned int k2 = k1 + 1;
		unsigned int k3 = k1 + 2;
		unsigned int k4 = k3 + 1;

		indices.push_back(k1);
		indices.push_back(k3);
		indices.push_back(k2);

		indices.push_back(k2);
		indices.push_back(k3);
		indices.push_back(k4);
	}

	// ----- TOP circle -----
	unsigned int baseIndex = vertices.size() / 8;
	vertices.push_back(0.0f); vertices.push_back(halfHeight); vertices.push_back(0.0f);
	vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
	vertices.push_back(0.5f); vertices.push_back(0.5f);
	unsigned int centerTopIndex = baseIndex++;

	for (int i = 0; i <= sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;
		float x = cosf(sectorAngle);
		float z = sinf(sectorAngle);

		vertices.push_back(radius * x);
		vertices.push_back(halfHeight);
		vertices.push_back(radius * z);
		vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
		vertices.push_back(0.5f + x * 0.5f);
		vertices.push_back(0.5f + z * 0.5f);

		if (i < sectorCount)
		{
			indices.push_back(centerTopIndex);
			indices.push_back(baseIndex + i + 1);
			indices.push_back(baseIndex + i);
		}
	}

	// ----- BOTTOM circle -----
	baseIndex = vertices.size() / 8;
	vertices.push_back(0.0f); vertices.push_back(-halfHeight); vertices.push_back(0.0f);
	vertices.push_back(0.0f); vertices.push_back(-1.0f); vertices.push_back(0.0f);
	vertices.push_back(0.5f); vertices.push_back(0.5f);
	unsigned int centerBottomIndex = baseIndex++;

	for (int i = 0; i <= sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;
		float x = cosf(sectorAngle);
		float z = sinf(sectorAngle);

		vertices.push_back(radius * x);
		vertices.push_back(-halfHeight);
		vertices.push_back(radius * z);
		vertices.push_back(0.0f); vertices.push_back(-1.0f); vertices.push_back(0.0f);
		vertices.push_back(0.5f + x * 0.5f);
		vertices.push_back(0.5f + z * 0.5f);

		if (i < sectorCount)
		{
			indices.push_back(centerBottomIndex);
			indices.push_back(baseIndex + i);
			indices.push_back(baseIndex + i + 1);
		}
	}
}
