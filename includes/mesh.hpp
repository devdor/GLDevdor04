#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// GL Includes
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"


struct Vertex {
	// Position
	glm::vec3 Position;
	// Normal
	glm::vec3 Normal;
	// TexCoords
	glm::vec2 TexCoords;
};

class CMesh {

public:
	/*  Mesh Data  */
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	void Release()
	{
	}

	CMesh() 
	{
	}

	CMesh(const CMesh &mesh)
	{
		this->vertices = mesh.vertices;
		this->indices = mesh.indices;
		this->SetupMesh();
	}

	CMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
	{
		this->vertices = vertices;
		this->indices = indices;

		this->SetupMesh();
	}

	void Init(std::string path)
	{
		// Read file via ASSIMP
		Assimp::Importer importer;
		
		//const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		
		// Check for errors
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}
		
		// Data to fill
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		// Walk through each of the mesh's vertices
		for (GLuint i = 0; i < scene->mMeshes[0]->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
							  // Positions
			vector.x = scene->mMeshes[0]->mVertices[i].x;
			vector.y = scene->mMeshes[0]->mVertices[i].y;
			vector.z = scene->mMeshes[0]->mVertices[i].z;
			vertex.Position = vector;
			// Normals
			vector.x = scene->mMeshes[0]->mNormals[i].x;
			vector.y = scene->mMeshes[0]->mNormals[i].y;
			vector.z = scene->mMeshes[0]->mNormals[i].z;
			vertex.Normal = vector;
			
			// Texture Coordinates
			if (scene->mMeshes[0]->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = scene->mMeshes[0]->mTextureCoords[0][i].x;
				vec.y = scene->mMeshes[0]->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			
			vertices.push_back(vertex);
		}

		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (GLuint i = 0; i < scene->mMeshes[0]->mNumFaces; i++)
		{
			aiFace face = scene->mMeshes[0]->mFaces[i];
			
			// Retrieve all indices of the face and store them in the indices vector
			for (GLuint j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		this->vertices = vertices;
		this->indices = indices;

		this->SetupMesh();
	}

	void Render()
	{
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	std::vector<Vertex> GetVertices()
	{
		return this->vertices;
	}

	Vertex GetVertex(int n)
	{
		return this->vertices[n];
	}

	void UpdateVertex(int n, glm::vec3 pos)
	{
		this->vertices[n].Position.x = pos.x;
		this->vertices[n].Position.y = pos.y;
		this->vertices[n].Position.z = pos.z;
	}


	/*  Render data  */
	GLuint VAO, VBO, EBO;

	void UpdateMesh()
	{
		glBindVertexArray(this->VAO);

		// Load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
	}

private:
	// Initializes all the buffer objects/arrays
	void SetupMesh()
	{
		// Create buffers/arrays
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);

		// Load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		// Set the vertex attribute pointers
		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
		// Vertex Texture Coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};
#endif