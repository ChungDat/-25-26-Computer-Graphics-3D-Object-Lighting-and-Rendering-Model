#include "Object.h"
#include "../utils.h"

// static definitions
int Object::nextID = 0;

unsigned int Cube::VAO = 0;
unsigned int Cube::VBO = 0;
bool Cube::initialized = false;

unsigned int Pyramid::VAO = 0;
unsigned int Pyramid::VBO = 0;
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

Object::Object(const std::string& diffusePath, const std::string& specularPath, const std::string& emssionPath) 
	: diffusePath(diffusePath), specularPath(specularPath), emissionPath(emssionPath), ID(nextID++)
{
	if (!diffusePath.empty()) {
		diffuseMap = createTexture(diffusePath.c_str());
	}
	else {
		diffuseMap = 0;
	}

	if (!specularPath.empty()) {
		specularMap = createTexture(specularPath.c_str());
	}
	else {
		specularMap = 0;
	}

	if (!emssionPath.empty()) {
		emissionMap = createTexture(emssionPath.c_str());
	}
	else {
		emissionMap = 0;
	}

	// default
	position = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	rotation = glm::vec3(0.0f);

	enabled = true;

	setModelMatrix();
}

Object::Object(const std::string& diffusePath, const std::string& specularPath) 
	: Object(diffusePath, specularPath, "") {}

Object::Object(const std::string& diffusePath) 
	: Object(diffusePath, "", "") {}

Object::Object() 
	: Object("", "", "") {}

void Object::draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos, const glm::mat4& horizontalRotate, const glm::mat4& verticalRotate) {
	if (!isEnabled()) return;
	shader.use();
	setModelMatrix();
	shader.setMat4fv("view", view);
	shader.setMat4fv("projection", projection);
	shader.setVec3fv("viewPos", viewPos);
	shader.setMat4fv("horizontalRotate", horizontalRotate);
	shader.setMat4fv("verticalRotate", verticalRotate);
	shader.setMat4fv("model", model);
	
	shader.setInt("material.diffuse", 0);
	shader.setInt("material.specular", 1);
	shader.setInt("material.emission", 2);

	glBindVertexArray(getVAO());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissionMap);

	glDrawArrays(getDrawMode(), 0, getVertexCount());
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

float Object::getX() const {
	return position.x;
}

float Object::getY() const {
	return position.y;
}

float Object::getZ() const {
	return position.z;
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

Cube::Cube(const std::string& diffusePath, const std::string& specularPath, const std::string& emssionPath) 
	: Object(diffusePath, specularPath, emssionPath)
{
	initBuffers();
}

Cube::Cube(const std::string& diffusePath, const std::string& specularPath) 
	: Cube(diffusePath, specularPath, "") {}

Cube::Cube(const std::string& diffusePath)
	: Cube(diffusePath, "", "") {}

Cube::Cube()
	: Cube("", "", "") {}

void Cube::initBuffers() {
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

unsigned int Cube::getVAO() const {
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

Pyramid::Pyramid(const std::string& diffusePath, const std::string& specularPath, const std::string& emssionPath)
	: Object(diffusePath, specularPath, emssionPath)
{
	initBuffers();
}

Pyramid::Pyramid(const std::string& diffusePath, const std::string& specularPath)
	: Pyramid(diffusePath, specularPath, "") {}

Pyramid::Pyramid(const std::string& diffusePath)
	: Pyramid(diffusePath, "", "") {}

Pyramid::Pyramid()
	: Pyramid("", "", "") {
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

    // Side 1 (A, B, E)
    -0.5f, 0.0f, -0.5f,   0.0f, 0.707f, -0.707f,   0.0f, 0.0f,  // A
     0.5f, 0.0f, -0.5f,   0.0f, 0.707f, -0.707f,   1.0f, 0.0f,  // B
     0.0f, 1.0f,  0.0f,   0.0f, 0.707f, -0.707f,   0.5f, 1.0f,  // E

    // Side 2 (B, C, E)
     0.5f, 0.0f, -0.5f,   0.707f, 0.707f, 0.0f,    0.0f, 0.0f,  // B
     0.5f, 0.0f,  0.5f,   0.707f, 0.707f, 0.0f,    1.0f, 0.0f,  // C
     0.0f, 1.0f,  0.0f,   0.707f, 0.707f, 0.0f,    0.5f, 1.0f,  // E

    // Side 3 (C, D, E)
     0.5f, 0.0f,  0.5f,   0.0f, 0.707f, 0.707f,    0.0f, 0.0f,  // C
    -0.5f, 0.0f,  0.5f,   0.0f, 0.707f, 0.707f,    1.0f, 0.0f,  // D
     0.0f, 1.0f,  0.0f,   0.0f, 0.707f, 0.707f,    0.5f, 1.0f,  // E

    // Side 4 (D, A, E)
    -0.5f, 0.0f,  0.5f,  -0.707f, 0.707f, 0.0f,    0.0f, 0.0f,  // D
    -0.5f, 0.0f, -0.5f,  -0.707f, 0.707f, 0.0f,    1.0f, 0.0f,  // A
     0.0f, 1.0f,  0.0f,  -0.707f, 0.707f, 0.0f,    0.5f, 1.0f,  // E

    // Base (A, B, C, D)
    -0.5f, 0.0f, -0.5f,   0.0f, -1.0f,  0.0f,      0.0f, 0.0f,  // A
     0.5f, 0.0f, -0.5f,   0.0f, -1.0f,  0.0f,      1.0f, 0.0f,  // B
     0.5f, 0.0f,  0.5f,   0.0f, -1.0f,  0.0f,      1.0f, 1.0f,  // C

     0.5f, 0.0f,  0.5f,   0.0f, -1.0f,  0.0f,      1.0f, 1.0f,  // C
    -0.5f, 0.0f,  0.5f,   0.0f, -1.0f,  0.0f,      0.0f, 1.0f,  // D
    -0.5f, 0.0f, -0.5f,   0.0f, -1.0f,  0.0f,      0.0f, 0.0f,  // A
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

unsigned int Pyramid::getVAO() const {
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

Sphere::Sphere(const std::string& diffusePath, const std::string& specularPath, const std::string& emssionPath)
	: Object(diffusePath, specularPath, emssionPath)
{
	initBuffers();
}

Sphere::Sphere(const std::string& diffusePath, const std::string& specularPath)
	: Sphere(diffusePath, specularPath, "") {}

Sphere::Sphere(const std::string& diffusePath)
	: Sphere(diffusePath, "", "") {}

Sphere::Sphere()
	: Sphere("", "", "") {}

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

unsigned int Sphere::getVAO() const {
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

void Sphere::draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos, const glm::mat4& horizontalRotate, const glm::mat4& verticalRotate) {
	if (!isEnabled()) return;
	shader.use();
	setModelMatrix();
	shader.setMat4fv("view", view);
	shader.setMat4fv("projection", projection);
	shader.setVec3fv("viewPos", viewPos);
	shader.setMat4fv("horizontalRotate", horizontalRotate);
	shader.setMat4fv("verticalRotate", verticalRotate);
	shader.setMat4fv("model", model);

	glBindVertexArray(getVAO());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissionMap);

	glDrawElements(getDrawMode(), getVertexCount(), GL_UNSIGNED_INT, 0);
}

// Cylinder class
// --------------

Cylinder::Cylinder(const std::string& diffusePath, const std::string& specularPath, const std::string& emssionPath)
	: Object(diffusePath, specularPath, emssionPath)
{
	initBuffers();
}

Cylinder::Cylinder(const std::string& diffusePath, const std::string& specularPath)
	: Cylinder(diffusePath, specularPath, "") {
}

Cylinder::Cylinder(const std::string& diffusePath)
	: Cylinder(diffusePath, "", "") {
}

Cylinder::Cylinder()
	: Cylinder("", "", "") {
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

unsigned int Cylinder::getVAO() const {
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

void Cylinder::draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos, const glm::mat4& horizontalRotate, const glm::mat4& verticalRotate) {
	if (!isEnabled()) return;
	shader.use();
	setModelMatrix();
	shader.setMat4fv("view", view);
	shader.setMat4fv("projection", projection);
	shader.setVec3fv("viewPos", viewPos);
	shader.setMat4fv("horizontalRotate", horizontalRotate);
	shader.setMat4fv("verticalRotate", verticalRotate);
	shader.setMat4fv("model", model);

	glBindVertexArray(getVAO());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissionMap);

	glDrawElements(getDrawMode(), getVertexCount(), GL_UNSIGNED_INT, 0);
}