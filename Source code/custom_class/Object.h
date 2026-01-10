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

struct Material{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float shininess;

    glm::vec3 albedo;
    float metallic;
    float roughness;
};

struct Texture {
    std::string diffusePath;
    std::string specularPath;
    std::string emissionPath;

    std::string albedoPath;
    std::string metallicPath;
    std::string roughnessPath;

    unsigned int diffuseMap;
    unsigned int specularMap;
    unsigned int emissionMap;

    unsigned int albedoMap;
    unsigned int metallicMap;
    unsigned int roughnessMap;
};

class Object
{
protected:
    glm::mat4 model;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation; // a vector representing rotation angles around x, y, z axes in degrees

    Material material;
    Texture texture;

    Shader*& shader;

    bool enabled;

    static int nextID;
    int ID;

    void setModelMatrix();
    virtual unsigned int getVAO() const = 0;
    virtual unsigned int getVertexCount() const = 0;
    virtual GLenum getDrawMode() const;

public:
    Object(Shader*&);

    virtual void draw(const glm::mat4& horizontalRotate, const glm::mat4& verticalRotate);
    virtual void draw();
    
    void setDiffusePath(const std::string);
    void setSpecularPath(const std::string);
    void setEmissionPath(const std::string);

    void setAlbedoPath(const std::string);
    void setMetallicPath(const std::string);
    void setRoughnessPath(const std::string);
    
    void setAmbient(const glm::vec3);
    void setDiffuse(const glm::vec3);
    void setSpecular(const glm::vec3);

    void setAlbedo(const glm::vec3);
    void setRoughness(const float);
    void setMetallic(const float);

    void setPosition(const glm::vec3 _pos);
    void setScale(const glm::vec3 _scale);
    void setRotation(const glm::vec3 _rotation);

    void disable();
    void enable();
    bool isEnabled() const;

    glm::vec3 getPosition() const;
    glm::vec3& getPosition_Ref();

    float getX() const;
    float getY() const;
    float getZ() const;

    glm::vec3 getAmbient() const;
    glm::vec3 getDiffuse() const;
    glm::vec3 getSpecular() const;

    float getShininess() const;

    glm::vec3 getAlbedo() const;
    float getRoughness() const;
    float getMetallic() const;

    int getID() const;

    virtual std::string getType() const = 0;
    
    virtual ~Object();
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
    Cube(Shader*&);
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
    Sphere(Shader*&);
    void draw(const glm::mat4& horizontalRotate, const glm::mat4& verticalRotate) override;
    void draw() override;
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
    Pyramid(Shader*&);
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
    Cylinder(Shader*&);
    void draw(const glm::mat4& horizontalRotate, const glm::mat4& verticalRotate) override;
    void draw() override;
};
