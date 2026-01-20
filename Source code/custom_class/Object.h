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
    float pitch; // along x
    float yaw; // along y
    float roll; // along z

    Material material;
    Texture texture;

    bool enabled;

    static int nextID;
    int ID;

    void setModelMatrix();
    virtual void initBuffers() = 0;
    virtual unsigned int getVAO() = 0;
    virtual unsigned int getVertexCount() const = 0;
    virtual GLenum getDrawMode() const;

public:
    Object();

    virtual void draw(Shader*&);
    
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
    void setRotation(const float _pitch, const float _yaw, const float _roll);

    void setYaw(const float _yaw);
    void setPitch(const float _pitch);
    void setRoll(const float _roll);

    void disable();
    void enable();
    bool isEnabled() const;

    glm::vec3 getPosition() const;
    glm::vec3& getPosition_Ref();
    glm::vec3 getScale() const;
    glm::vec3 getRotation() const;
    float getYaw() const;
    float getPitch() const;
    float getRoll() const;

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

    void initBuffers() override;
    unsigned int getVAO() override;
    unsigned int getVertexCount() const override;
    std::string getType() const override;

public:
    Cube();
};

class Sphere : public Object {
protected:
    static unsigned int VAO, VBO, EBO;
    static std::vector<float> vertices;
    static std::vector<unsigned int> indices;
    static bool initialized;

    void initBuffers() override;
    unsigned int getVAO() override;
    unsigned int getVertexCount() const override;
    std::string getType() const override;
    static void generateSphere(float radius, unsigned int sectorCount, unsigned int stackCount,
        std::vector<float>& vertices, std::vector<unsigned int>& indices);

public:
    Sphere();
};

class Pyramid : public Object {
protected:
    static unsigned int VAO, VBO, EBO;
    static bool initialized;

    void initBuffers() override;
    unsigned int getVAO() override;
    unsigned int getVertexCount() const override;
    std::string getType() const override;

public:
    Pyramid();
};

class Cylinder : public Object {
protected:
    static unsigned int VAO, VBO, EBO;
    static std::vector<float> vertices;
    static std::vector<unsigned int> indices;
    static bool initialized;

    void initBuffers() override;
    unsigned int getVAO() override;
    unsigned int getVertexCount() const override;
    std::string getType() const override;
    static void generateCylinder(float radius, float height, unsigned int sectorCount,
        std::vector<float>& vertices, std::vector<unsigned int>& indices);

public:
    Cylinder();
};
