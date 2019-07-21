#ifndef SCREEN_PLANE_HPP
#define SCREEN_PLANE_HPP

#include <glad/glad.h>

class CScreenPlane
	{
	public:
		CScreenPlane();
		CScreenPlane(GLfloat size);
		~CScreenPlane();

		void Init(GLfloat size);
		void Bind();
		void Render();
		void Release();

		GLuint GetVAO();
		int GetNumVertices();

	private:
		GLuint m_VAO;
		int m_numVertices;
	};
#endif