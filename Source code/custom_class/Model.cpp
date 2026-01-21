#include "Model.h"

// static definitions

bool Backpack::initialized = false;
std::vector<Mesh> Backpack::meshes = std::vector<Mesh>();
std::vector<ModelTexture> Backpack::textures_loaded = std::vector<ModelTexture>();
std::string Backpack::directory = std::string();

bool Chair::initialized = false;
std::vector<Mesh> Chair::meshes = std::vector<Mesh>();
std::vector<ModelTexture> Chair::textures_loaded = std::vector<ModelTexture>();
std::string Chair::directory = std::string();

Model::Model() : Object() {}

Model::~Model() {}

//void Model::draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos, const glm::mat4& horizontalRotate, const glm::mat4& verticalRotate) {
void Model::drawFrom(Shader*& shader, std::vector<Mesh> meshes, bool isShadowMapping) {
	if (!isEnabled()) return;

	setModelMatrix();

	shader->use();

	shader->setMat4fv("model", model);
	
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(*shader);
	}
}

void Model::drawFrom(Shader*& shader, std::vector<Mesh> meshes) {
	if (!isEnabled()) return;

	setModelMatrix();

	shader->use();

	shader->setMat4fv("model", model);
	
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(*shader);
	}
}

//void Model::loadModel(std::string path) {
//	loadModelTo(path, meshes, textures_loaded, directory);
//}

void Model::loadModelTo(const std::string& path,
	std::vector<Mesh>& out_meshes,
	std::vector<ModelTexture>& out_textures_loaded,
	std::string& out_directory)
{
	// read file via ASSIMP
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	// retrieve the directory path of the filepath
	out_directory = path.substr(0, path.find_last_of('/'));
	// process ASSIMP's root node recursively into the provided containers
	processNodeTo(scene->mRootNode, scene, out_meshes, out_textures_loaded, out_directory);
}

// processes a node in a recursive fashion. Processes each individual mesh located 
// at the node and repeats this process on its children nodes (if any).
//void Model::processNode(aiNode* node, const aiScene* scene) {
//	processNodeTo(node, scene, meshes, textures_loaded, directory);
//}

void Model::processNodeTo(aiNode* node, const aiScene* scene,
	std::vector<Mesh>& out_meshes,
	std::vector<ModelTexture>& out_textures_loaded,
	const std::string& out_directory)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		out_meshes.push_back(processMeshTo(mesh, scene, out_textures_loaded, out_directory));
	}

	// recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNodeTo(node->mChildren[i], scene, out_meshes, out_textures_loaded, out_directory);
	}
}

//Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
//	return processMeshTo(mesh, scene, textures_loaded, directory);
//}

Mesh Model::processMeshTo(aiMesh* mesh, const aiScene* scene,
	std::vector<ModelTexture>& out_textures_loaded,
	const std::string& out_directory)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<ModelTexture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		if (mesh->HasNormals()) {
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		if (mesh->mTextureCoords[0]) { // if mesh contains texture coordinates
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0, 0.0);
		}
		vertices.push_back(vertex);
	}
	// now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	// process material
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// 1. diffuse maps
		std::vector<ModelTexture> diffuseMaps = loadMaterialTexturesTo(material, aiTextureType_DIFFUSE, "texture_diffuse", out_textures_loaded, out_directory);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<ModelTexture> specularMaps = loadMaterialTexturesTo(material, aiTextureType_SPECULAR, "texture_specular", out_textures_loaded, out_directory);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<ModelTexture> normalMaps = loadMaterialTexturesTo(material, aiTextureType_HEIGHT, "texture_normal", out_textures_loaded, out_directory);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<ModelTexture> heightMaps = loadMaterialTexturesTo(material, aiTextureType_AMBIENT, "texture_height", out_textures_loaded, out_directory);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}
	return Mesh(vertices, indices, textures);
}



// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
//std::vector<ModelTexture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
//	return loadMaterialTexturesTo(mat, type, typeName, textures_loaded, directory);
//}

std::vector<ModelTexture> Model::loadMaterialTexturesTo(aiMaterial* mat, aiTextureType type, std::string typeName,
	std::vector<ModelTexture>& out_textures_loaded,
	const std::string& out_directory)
{
	std::vector<ModelTexture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < out_textures_loaded.size(); j++)
		{
			if (std::strcmp(out_textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(out_textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			ModelTexture texture;
			texture.id = TextureFromFile(str.C_Str(), out_directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			out_textures_loaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}

unsigned int Model::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

unsigned int Model::getVAO() {
	initBuffers();
	return 0;
}

unsigned int Model::getVertexCount() const {
	return 0;
}

Backpack::Backpack() : Model() {
	initBuffers();
}

Backpack::~Backpack() {}

std::string Backpack::getType() const {
	return "Backpack";
}

void Backpack::initBuffers() {
	if (initialized) return;

	loadModelTo("assimp/backpack/backpack.obj", meshes, textures_loaded, directory);
	
	initialized = true;
}

unsigned int Backpack::getVAO() {
	return 0;
}

unsigned int Backpack::getVertexCount() const {
	return 0;
}

void Backpack::draw(Shader*& shader) {
	drawFrom(shader, meshes);
}

void Backpack::draw(Shader*& shader, bool isShadowMapping) {
	drawFrom(shader, meshes);
}

Chair::Chair() : Model() {
	initBuffers();
	scale = glm::vec3(0.2f, 0.2f, 0.2f);
}

Chair::~Chair() {}

std::string Chair::getType() const {
	return "Chair";
}

void Chair::initBuffers() {
	if (initialized) return;

	loadModelTo("assimp/chair/modern chair.obj", meshes, textures_loaded, directory);

	initialized = true;
}

unsigned int Chair::getVAO() {
	return 0;
}

unsigned int Chair::getVertexCount() const {
	return 0;
}

void Chair::draw(Shader*& shader) {
	drawFrom(shader, meshes);
}

void Chair::draw(Shader*& shader, bool isShadowMapping) {
	drawFrom(shader, meshes);
}