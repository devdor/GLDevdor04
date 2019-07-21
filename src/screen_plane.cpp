#include "../includes/screen_plane.hpp"

CScreenPlane::CScreenPlane()
{
}

CScreenPlane::CScreenPlane(GLfloat size)
{
	this->Init(size);
}

CScreenPlane::~CScreenPlane()
{
}

void CScreenPlane::Init(GLfloat fSize)
{
	// Setup plane VAO
	GLfloat vertices[] = {
		-fSize, fSize, 0.0f, fSize,
		-fSize, -fSize, 0.0f, 0.0f,
		fSize, -fSize, fSize, 0.0f,

		-fSize, fSize, 0.0f, fSize,
		fSize, -fSize, fSize, 0.0f,
		fSize, fSize, fSize, fSize
	};

	this->m_numVertices = sizeof(vertices);

	// Setup screen VAO
	GLuint quadVBO;
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);
}


void CScreenPlane::Bind()
{
	glBindVertexArray(this->m_VAO);
}

int CScreenPlane::GetNumVertices()
{
	return this->m_numVertices;
}

GLuint CScreenPlane::GetVAO()
{
	return this->m_VAO;
}

void CScreenPlane::Render()
{
	glDrawArrays(GL_TRIANGLES, 0, this->m_numVertices);
}

void CScreenPlane::Release()
{
	glDeleteVertexArrays(1, &this->m_VAO);
}