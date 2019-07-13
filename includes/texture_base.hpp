#ifndef TEXTURE_BASE_HPP
#define TEXTURE_BASE_HPP

#include <glad/glad.h>

class CTextureBase
{
public:
	CTextureBase();
	GLuint ID;
	
	void Release() const;
	void Activate() const;
	void Activate(int glTextureStage) const;

	void Bind() const;
	void UnBind() const;
};
#endif