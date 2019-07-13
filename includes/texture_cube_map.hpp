#ifndef TEXTURE_CUBE_MAP_HPP
#define TEXTURE_CUBE_MAP_HPP

#include <vector>
#include <glad/glad.h>
#include "texture_base.hpp"

class CTextureCubeMap : public CTextureBase
{
public:
	CTextureCubeMap();	

	void GenerateDepth(GLuint width, GLuint height);
	void GenerateTex(GLuint width, GLuint height, GLenum format, std::vector<unsigned char*> data);

	void Bind();
	void Activate();
	void Activate(int glTextureStage);
};
#endif