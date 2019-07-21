#ifndef SCENE02_HPP
#define SCENE02_HPP

#include "base_scene.hpp"
#include "util.hpp"
#include "import_mesh.hpp"
#include "cube_mesh.hpp"
#include "plane_mesh.hpp"
#include "texture2d.hpp"
#include "scene_update_args.hpp"
#include "particle_emitter.hpp"
#include "metaballs_util.hpp"
#include "frame_buffer_depth.hpp"
#include "frame_buffer_tex.hpp"

class CScene02 : public CBaseScene
{
public:
    CScene02();
    ~CScene02();

    void Init(CSceneInitArgs &args);
    void SetGlStates();
    void Render(CSceneUpdateArgs &args);
    void Update(CSceneUpdateArgs &args);
    void Release();

private:
    void RenderCommon(CSceneUpdateArgs &args, const CShader &shader);
    void WaveFuncObject(CImportMesh &mesh, const float curFrame);

    // shader
    CShader m_shader, m_simpleDepthShader, m_shaderCubeMapReflect;
    
    // framebuffer
    CFrameBufferDepth m_frameBufferDepth;

    // lights
    glm::vec3 m_lightPos;
    
    // meshes
    CPlaneMesh m_planeMesh;
    CImportMesh m_importMesh1, m_importMesh2;

    // textures
    CTexture2D m_texFloor, m_texRed;
    CTextureCubeMap m_texCubeMap;
};
#endif