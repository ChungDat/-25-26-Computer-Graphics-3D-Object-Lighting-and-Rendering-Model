#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include "Shader.h"
#include "Mesh.h"

class Model
{
public:
	Model(char*);
	void draw(Shader&);
private:
	// model data
	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded;
	std::string directory;
	bool gammaCorrection;

	void loadModel(std::string);
	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	std::vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, std::string);
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
};

