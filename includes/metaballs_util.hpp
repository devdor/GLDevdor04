#ifndef METABALLS_UTIL_HPP
#define METABALLS_UTIL_HPP

#include <vector>
#include <glad/glad.h>
#include "vertex.hpp"
#include "scene_update_args.hpp"
#include "metaballs/metaball.hpp"
#include "metaballs/champ.hpp"

class CMetaballsUtil 
{
    public:
        void Init()
        {
            this->m_champ = new CChamp();
            glGenVertexArrays(1, &this->m_vAO);
            glGenBuffers(1, &m_vBO);

            glBindBuffer(GL_ARRAY_BUFFER, this->m_vBO);
            glBufferData(GL_ARRAY_BUFFER, this->m_metaballVertices.size() * sizeof(Vertex), &this->m_metaballVertices[0], GL_STATIC_DRAW);

            glBindVertexArray(this->m_vAO);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);
            glBindVertexArray(0);
        };
        
        void Update(CSceneUpdateArgs &args)
        {
            // Update Positions
            CMetaball mballList[NUM_BALLS];
            for (int n = 0; n < NUM_BALLS; n++)
            {
                mballList[n].UpdateCenter(
                    16 + 8 * sin(n + args.GetCurrentFrame() * 0.3),
                    16 + 8 * cos(2 * n + args.GetCurrentFrame() * 0.5),
                    16 + 8 * sin(2 * n + args.GetCurrentFrame() * 0.3));
            }

            this->m_champ->Calculate((CMetaball *)&mballList, NUM_BALLS);
            this->m_champ->TriangleOptimization((CMetaball *)&mballList, NUM_BALLS, 0.2f);

            // Calculate VertexBuffer
            this->m_metaballVertices.clear();
            this->m_metaballVertices.reserve(this->m_champ->nbPoints);
            for (int i = 0; i < this->m_champ->nbPoints; i++)
            {
                Vertex vertex;
                vertex.Position = this->m_champ->pointsMesh[i];
                vertex.Normal = this->m_champ->normalesMesh[i];
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
                this->m_metaballVertices.push_back(vertex);
            }
            
            glBindBuffer(GL_ARRAY_BUFFER, m_vBO);
	        glBufferData(GL_ARRAY_BUFFER, this->m_metaballVertices.size() * sizeof(Vertex), &this->m_metaballVertices[0], GL_STATIC_DRAW);
        };

        void Render()
        {
            glBindVertexArray(this->m_vAO);
            glDrawArrays(GL_TRIANGLES, 0, this->m_metaballVertices.size());
            glBindVertexArray(0);
        };

        void Release()
        {
            glDeleteVertexArrays(1, &this->m_vAO);
    	    glDeleteBuffers(1, &m_vBO);
            delete(this->m_champ);
        };

    private:
        const static int NUM_BALLS = 16;

        CChamp* m_champ;        
        GLuint m_vBO, m_vAO;
        std::vector<Vertex> m_metaballVertices;
};
#endif