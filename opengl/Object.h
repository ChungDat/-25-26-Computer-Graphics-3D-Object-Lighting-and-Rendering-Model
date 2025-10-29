#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <cmath>

class Object
{
public:
    Object(Shader& shader);
	Object(Shader& shader, const std::string& diffusePath);
	Object(Shader& shader, const std::string& diffusePath, const std::string& specularPath);
	Object(Shader& shader, const std::string& diffusePath, const std::string& specularPath, const std::string& emssionPath);
    //Object(const Object& other);

    virtual void draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos, const glm::mat4& horizontalRotate, const glm::mat4& verticalRotate);
    void setPosition(const glm::vec3 _pos);
    void setScale(const glm::vec3 _scale);
    void setRotation(const glm::vec3 _rotation);
protected:
    Shader& shader;
    unsigned int diffuseMap, specularMap, emissionMap;
    std::string diffusePath, specularPath, emissionPath;
    glm::mat4 model;
    glm::vec3 position;
    glm::vec3 scale;
	glm::vec3 rotation; // a vector representing rotation angles around x, y, z axes in degrees

    void setModelMatrix();
    virtual unsigned int getVAO() const = 0;
    virtual unsigned int getVertexCount() const = 0;
    virtual GLenum getDrawMode() const;
};

class Cube : public Object {
protected:
    static unsigned int VAO, VBO, EBO;
    static bool initialized;
    static void initBuffers();
    unsigned int getVAO() const;
    unsigned int getVertexCount() const;
public:
    Cube(Shader& shader);
    Cube(Shader& shader, const std::string& diffusePath);
    Cube(Shader& shader, const std::string& diffusePath, const std::string& specularPath);
    Cube(Shader& shader, const std::string& diffusePath, const std::string& specularPath, const std::string& emssionPath);
};

class Sphere : public Object {
protected:
    static unsigned int VAO, VBO, EBO;
    static std::vector<float> vertices;
    static std::vector<unsigned int> indices;
    static bool initialized;
    static void initBuffers();
    unsigned int getVAO() const;
    unsigned int getVertexCount() const;
    static void generateSphere(float radius, unsigned int sectorCount, unsigned int stackCount,
        std::vector<float>& vertices, std::vector<unsigned int>& indices);
public:
    Sphere(Shader& shader);
    Sphere(Shader& shader, const std::string& diffusePath);
    Sphere(Shader& shader, const std::string& diffusePath, const std::string& specularPath);
    Sphere(Shader& shader, const std::string& diffusePath, const std::string& specularPath, const std::string& emssionPath);
    void draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos, const glm::mat4& horizontalRotate, const glm::mat4& verticalRotate) override;
};

class Pyramid : public Object {
protected:
    static unsigned int VAO, VBO, EBO;
    static bool initialized;
    static void initBuffers();
    unsigned int getVAO() const;
    unsigned int getVertexCount() const;
public:
    Pyramid(Shader& shader);
    Pyramid(Shader& shader, const std::string& diffusePath);
    Pyramid(Shader& shader, const std::string& diffusePath, const std::string& specularPath);
    Pyramid(Shader& shader, const std::string& diffusePath, const std::string& specularPath, const std::string& emssionPath);
};

class Cylinder : public Object {
protected:
    static unsigned int VAO, VBO, EBO;
    static std::vector<float> vertices;
    static std::vector<unsigned int> indices;
    static bool initialized;
    static void initBuffers();
    unsigned int getVAO() const;
    unsigned int getVertexCount() const;
    static void generateCylinder(float radius, float height, unsigned int sectorCount,
        std::vector<float>& vertices, std::vector<unsigned int>& indices);
public:
    Cylinder(Shader& shader);
    Cylinder(Shader& shader, const std::string& diffusePath);
    Cylinder(Shader& shader, const std::string& diffusePath, const std::string& specularPath);
    Cylinder(Shader& shader, const std::string& diffusePath, const std::string& specularPath, const std::string& emssionPath);
    void draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos, const glm::mat4& horizontalRotate, const glm::mat4& verticalRotate) override;
};
