#include "../includes/texture_base.hpp"

CTextureBase::CTextureBase()
{
	glGenTextures(1, &this->ID);
}

void CTextureBase::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

void CTextureBase::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CTextureBase::Activate() const
{
	this->Activate(GL_TEXTURE0);
}

void CTextureBase::Activate(int glTextureStage) const
{
	glActiveTexture(glTextureStage);
	this->Bind();
}

void CTextureBase::Release() const
{
	glDeleteTextures(1, &this->ID);
}
