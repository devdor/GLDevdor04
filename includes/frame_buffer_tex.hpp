#ifndef FRAME_BUFFER_TEX_HPP
#define FRAME_BUFFER_TEX_HPP

#include "texture2d.hpp"

class CFrameBufferTexture
{
public:
	CFrameBufferTexture();
		
	void Generate(GLuint width, GLuint height)
	{
		this->Generate(width, height, false);
	};

	void Generate(GLuint width, GLuint height, GLboolean alpha)
	{
		this->m_RenderTexture.GenerateRenderTexture(width, height, alpha);
		this->ID = this->CreateFrameBuffer(this->m_RenderTexture.ID, width, height);
	};
	
	void Texture2DUpdate()
	{
		glBindTexture(GL_TEXTURE_2D, this->m_RenderTexture.ID);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	};

	void Texture2DBind()
	{
		this->m_RenderTexture.Bind();
	};

	void Texture2DActivate()
	{
		this->Texture2DActivate(GL_TEXTURE0);
	};

	void Texture2DActivate(int glTextureStage)
	{
		this->m_RenderTexture.Activate(glTextureStage);
		this->Texture2DBind();
	};

	void Release()
	{
		this->m_RenderTexture.Release();
	};

	GLuint GetRenderTextureId()
	{
		return this->m_RenderTexture.ID;
	}

	GLuint ID;

private:
	CTexture2D m_RenderTexture;
	GLuint CreateFrameBuffer(GLuint textureId, GLsizei width, GLsizei height)
	{
		// create a renderbuffer object to store depth info
		GLuint rboId;
		glGenRenderbuffers(1, &rboId);
		glBindRenderbuffer(GL_RENDERBUFFER, rboId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
			width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// create a framebuffer object
		GLuint fboId;
		glGenFramebuffers(1, &fboId);
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);

		// attach the texture to FBO color attachment point
		glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER 
			GL_COLOR_ATTACHMENT0,  // 2. attachment point
			GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
			textureId,             // 4. tex ID
			0);                    // 5. mipmap level: 0(base)

								// attach the renderbuffer to depth attachment point
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
			GL_DEPTH_ATTACHMENT, // 2. attachment point
			GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
			rboId);              // 4. rbo ID

								// check FBO status
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		
		if (status != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		// switch back to window-system-provided framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return fboId;
	};
};
#endif