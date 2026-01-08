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
    Object();
	Object(const std::string& diffusePath);
	Object(const std::string& diffusePath, const std::string& specularPath);
	Object(const std::string& diffusePath, const std::string& specularPath, const std::string& emssionPath);
    //Object(const Object& other);

    virtual void draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos, const glm::mat4& horizontalRotate, const glm::mat4& verticalRotate);
    void setPosition(const glm::vec3 _pos);
    void setScale(const glm::vec3 _scale);
    void setRotation(const glm::vec3 _rotation);
    void setAlbedo(const glm::vec3);
    void setRoughness(const float);
    void setMetallic(const float);
    void disable();
    void enable();
    bool isEnabled() const;

    glm::vec3 getPosition() const;
    glm::vec3& getPosition_Ref();
    float getX() const;
    float getY() const;
    float getZ() const;
    glm::vec3 getAlbedo() const;
    float getRoughness() const;
    float getMetallic() const;
    int getID() const;

    virtual std::string getType() const = 0;
    
    virtual ~Object();
protected:
    unsigned int diffuseMap, specularMap, emissionMap;
    std::string diffusePath, specularPath, emissionPath;
    glm::mat4 model;
    glm::vec3 position;
    glm::vec3 scale;
	glm::vec3 rotation; // a vector representing rotation angles around x, y, z axes in degrees
    glm::vec3 albedo;
    float roughness;
    float metallic;
    bool enabled;

    static int nextID;
    int ID;

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
    std::string getType() const;
public:
    Cube();
    Cube(const std::string& diffusePath);
    Cube(const std::string& diffusePath, const std::string& specularPath);
    Cube(const std::string& diffusePath, const std::string& specularPath, const std::string& emssionPath);
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
    std::string getType() const;
    static void generateSphere(float radius, unsigned int sectorCount, unsigned int stackCount,
        std::vector<float>& vertices, std::vector<unsigned int>& indices);
public:
    Sphere();
    Sphere(const std::string& diffusePath);
    Sphere(const std::string& diffusePath, const std::string& specularPath);
    Sphere(const std::string& diffusePath, const std::string& specularPath, const std::string& emssionPath);
    void draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos, const glm::mat4& horizontalRotate, const glm::mat4& verticalRotate) override;
};

class Pyramid : public Object {
protected:
    static unsigned int VAO, VBO, EBO;
    static bool initialized;
    static void initBuffers();
    unsigned int getVAO() const;
    unsigned int getVertexCount() const;
    std::string getType() const;
public:
    Pyramid();
    Pyramid(const std::string& diffusePath);
    Pyramid(const std::string& diffusePath, const std::string& specularPath);
    Pyramid(const std::string& diffusePath, const std::string& specularPath, const std::string& emssionPath);
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
    std::string getType() const;
    static void generateCylinder(float radius, float height, unsigned int sectorCount,
        std::vector<float>& vertices, std::vector<unsigned int>& indices);
public:
    Cylinder();
    Cylinder(const std::string& diffusePath);
    Cylinder(const std::string& diffusePath, const std::string& specularPath);
    Cylinder(const std::string& diffusePath, const std::string& specularPath, const std::string& emssionPath);
    void draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos, const glm::mat4& horizontalRotate, const glm::mat4& verticalRotate) override;
};
