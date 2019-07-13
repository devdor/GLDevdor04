#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <glad/glad.h>
#include "texture_base.hpp"

class CTexture2D : public CTextureBase
{
public:
	CTexture2D();

	GLuint Width, Height;
					   	
	void GenerateFromFile(GLuint width, GLuint height, GLenum format, unsigned char* data);
	void GenerateRenderTexture(GLuint width, GLuint height, GLboolean alpha);
	void GenerateDepthMapTexture(GLuint shadowWidth, GLuint shadowHeight);
};
#endif