#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include "Shader.h"
#include "Mesh.h"
#include "Object.h"

class Model : public Object {
protected:
	// model data
	//std::vector<Mesh> meshes;
	//std::vector<ModelTexture> textures_loaded;
	//std::string directory;

	bool gammaCorrection;

	//void loadModel(std::string);
	//void processNode(aiNode*, const aiScene*);
	//Mesh processMesh(aiMesh*, const aiScene*);
	std::vector<ModelTexture> loadMaterialTextures(aiMaterial*, aiTextureType, std::string);
	unsigned int TextureFromFile(const char*, const std::string&, bool = false);

	void loadModelTo(const std::string& path,
	                 std::vector<Mesh>& out_meshes,
	                 std::vector<ModelTexture>& out_textures_loaded,
	                 std::string& out_directory);

	void processNodeTo(aiNode* node, const aiScene* scene,
	                   std::vector<Mesh>& out_meshes,
	                   std::vector<ModelTexture>& out_textures_loaded,
	                   const std::string& out_directory);

	Mesh processMeshTo(aiMesh* mesh, const aiScene* scene,
	                   std::vector<ModelTexture>& out_textures_loaded,
	                   const std::string& out_directory);

	std::vector<ModelTexture> loadMaterialTexturesTo(aiMaterial* mat,
	                                                 aiTextureType type,
	                                                 std::string typeName,
	                                                 std::vector<ModelTexture>& out_textures_loaded,
	                                                 const std::string& out_directory);

	virtual void initBuffers() override = 0;
	unsigned int getVAO() override = 0;
	unsigned int getVertexCount() const override = 0;

	void drawFrom(Shader*&, std::vector<Mesh>);

public:
	Model();
	virtual ~Model();

	std::string getType() const override = 0;

};

class Backpack : public Model {
protected:
	static bool initialized;
	static std::vector<Mesh> meshes;
	static std::vector<ModelTexture> textures_loaded;
	static std::string directory;

	void initBuffers() override;
	unsigned int getVAO() override;
	unsigned int getVertexCount() const override;

public:
	Backpack();
	virtual ~Backpack();

	std::string getType() const override;

	void draw(Shader*&);
};