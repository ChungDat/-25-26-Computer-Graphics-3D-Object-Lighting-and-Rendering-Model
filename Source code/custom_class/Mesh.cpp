#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<ModelTexture> _textures) {
	vertices = _vertices;
	indices = _indices;
	textures = _textures;

	setupMesh();
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	// ids
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

	// weights
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
	glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader) {
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	// Reserve lower texture units (0..5) for simple geometry objects.
	// Reserve (6..13) for lights depth map.
	// Assimp model textures will start at this offset to avoid clashing.
	const int MODEL_TEXTURE_UNIT_OFFSET = 14;

	// Track whether model provides diffuse/specular
	bool hasDiffuse = false;
	bool hasSpecular = false;
	bool hasNormal = false;
	bool hasHeight = false;

	int firstDiffuseUnit = -1;
	int firstSpecularUnit = -1;
	int firstNormalUnit = -1;
	int firstHeightUnit = -1;

	for (unsigned int i = 0; i < textures.size(); i++) {
		int unit = MODEL_TEXTURE_UNIT_OFFSET + static_cast<int>(i); // texture unit index
		glActiveTexture(GL_TEXTURE0 + unit);

		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse") {
			number = std::to_string(diffuseNr++);
			if (!hasDiffuse) {
				hasDiffuse = true;
				firstDiffuseUnit = unit;
			}
		}
		else if (name == "texture_specular") {
			number = std::to_string(specularNr++); // transfer unsigned int to string
			if (!hasSpecular) {
				hasSpecular = true;
				firstSpecularUnit = unit;
			}
		}
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to string
			if (!hasNormal) {
				hasNormal = true;
				firstNormalUnit = unit;
			}
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to string
			if (!hasHeight) {
				hasHeight = true;
				firstHeightUnit = unit;
			}

		std::string uniformName = name + number;
		shader.setInt(uniformName.c_str(), unit);

		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	if (firstDiffuseUnit != -1) {
		shader.setInt("diffuseMap", firstDiffuseUnit);
		shader.setInt("albedoMap", firstDiffuseUnit);
	}
	else {
		shader.setInt("diffuseMap", 0);
		shader.setInt("albedoMap", 0);
	}

	if (firstSpecularUnit != -1) {
		shader.setInt("specularMap", firstSpecularUnit);
		shader.setInt("metallicMap", firstSpecularUnit); // if model uses metallic in specular slot
	}
	else {
		shader.setInt("specularMap", 0);
		shader.setInt("metallicMap", 0);
	}

	if (firstNormalUnit != -1) {
		shader.setInt("normalMap", firstNormalUnit);
	}
	else {
		shader.setInt("normalMap", 0);
	}

	shader.setBool("useDiffuseMap", hasDiffuse);
	shader.setBool("useSpecularMap", hasSpecular);

	// If your PBR shader uses useAlbedoMap/useMetallicMap/useRoughnessMap, update those too.
	shader.setBool("useAlbedoMap", hasDiffuse);
	shader.setBool("useMetallicMap", hasSpecular); // best-effort mapping
	shader.setBool("useRoughnessMap", hasHeight || false);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}