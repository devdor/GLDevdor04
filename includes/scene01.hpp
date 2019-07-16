#ifndef SCENE01_HPP
#define SCENE01_HPP

#include <vector>
#include "metaballs/metaball.hpp"
#include "metaballs/champ.hpp"
#include "base_scene.hpp"
#include "util.hpp"
#include "mesh.hpp"
#include "texture2d.hpp"
#include "scene_update_args.hpp"

class CScene01 : public CBaseScene
{
public:
    CScene01();
    ~CScene01();

    void Init(CSceneInitArgs &args);
    void SetGlStates();
    void Render(CSceneUpdateArgs &args);
    void Update(CSceneUpdateArgs &args);
    void Release();

private:
    // shader
    CShader m_shader, m_simpleDepthShader, m_debugDepthQuad;

    unsigned int m_depthMapFBO;
    unsigned int m_depthMap;

    // lights
    glm::vec3 m_lightPos;

    // meshes
    unsigned int m_planeVAO;
    unsigned int m_planeVBO;

    // Cube mesh
    unsigned int m_cubeVAO = 0;
    unsigned int m_cubeVBO = 0;
    
    // textures
    CTexture2D m_texFloor, m_texBlue, m_texRed;
    CTextureCubeMap m_texCubeMap;
    
    void RenderCommonObjects(CSceneUpdateArgs &args, const CShader &shader);
    void RenderCube();

    CMesh m_mesh;

    // Metaballs
    CChamp* m_champ;
	const static int NUM_BALLS = 16;
	GLuint VBO, m_vaoMetaballs;
	std::vector<Vertex> m_MetaballVertices;

    CShader m_shaderCubeMapReflect;

    void InitMetaballsVertexBuffer();
	void UpdateMetaballsVertexBuffer();
	void CalculateMetaballs(CSceneUpdateArgs &args);
    void RenderMetaballs(CSceneUpdateArgs &args, const CShader &shader);	
    unsigned int LoadCubemap(std::vector<std::string> faces);
};
#endif