#ifndef PLANE_MESH_HPP
#define PLANE_MESH_HPP

class CPlaneMesh {

public:
    void Init()
    {
        glGenVertexArrays(1, &this->m_vAO);
        glGenBuffers(1, &this->m_vBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, this->m_vBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_meshVertices), m_meshVertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(m_vAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    };

    void Render()
    {
        glBindVertexArray(this->m_vAO);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(m_meshVertices) / 8);
        glBindVertexArray(0);
    };

    void Release()
    {
        glDeleteVertexArrays(1, &this->m_vAO);
    	glDeleteBuffers(1, &m_vBO);
    }

private:
    unsigned int m_vAO = 0;
    unsigned int m_vBO = 0;

    float m_meshVertices[48] =
    {
        // positions            // normals         // texcoords
        25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

        25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
        25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };
};
#endif