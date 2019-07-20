#ifndef SCENE01_HPP
#define SCENE01_HPP

#include <vector>
#include <map>
#include "base_scene.hpp"
#include "util.hpp"
#include "import_mesh.hpp"
#include "cube_mesh.hpp"
#include "plane_mesh.hpp"
#include "texture2d.hpp"
#include "scene_update_args.hpp"
#include "particle_emitter.hpp"
#include "metaballs_util.hpp"
#include "frame_buffer.hpp"

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
    CShader m_shader, m_simpleDepthShader, m_shaderCubeMapReflect;
    
    // framebuffer
    CFrameBuffer m_frameBuffer;

    // lights
    glm::vec3 m_lightPos;
    
    // meshes
    CPlaneMesh m_planeMesh;
    CCubeMesh m_cubeMesh;
    CImportMesh m_importMesh;

    // textures
    CTexture2D m_texFloor, m_texBlue, m_texRed;
    CTextureCubeMap m_texCubeMap;
        
    // metaballs
    CMetaballsUtil m_metaBallsUtil;
    glm::mat4 m_metaBallsModel;    

    void RenderCommonObjects(CSceneUpdateArgs &args, const CShader &shader);
};
#endif