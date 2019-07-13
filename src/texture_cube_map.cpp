#include "../includes/texture_cube_map.hpp"

CTextureCubeMap::CTextureCubeMap()
	: CTextureBase()
{
}

void CTextureCubeMap::Bind() 
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
}

void CTextureCubeMap::Activate() 
{
	this->Activate(GL_TEXTURE0);
}

void CTextureCubeMap::Activate(int glTextureStage) 
{
	glActiveTexture(glTextureStage);
	this->Bind();
}

void CTextureCubeMap::GenerateTex(GLuint width, GLuint height, GLenum format, std::vector<unsigned char*> data)
{
	glGenTextures(1, &this->ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
	for (GLuint i = 0; i < data.size(); i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data[i]);
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CTextureCubeMap::GenerateDepth(GLuint width, GLuint height)
{
	// Create depth cubemap texture	
	glGenTextures(1, &this->ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
	for (GLuint i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}