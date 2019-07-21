#include "../includes/scene01.hpp"
#include "stb_image.hpp"

CScene01::CScene01()
    :CBaseScene()
{     
}

CScene01::~CScene01()
{    
}

void CScene01::Init(CSceneInitArgs &args)
{
    // camera
    this->m_camUtil.InitView(args.GetScreenSettings());
    this->m_camUtil.PathInit(0.015f,
        CUtil::LoadSpline(CFileSystem::GetPath("res/splines/eye_spline01.txt").c_str()),
        CUtil::LoadSpline(CFileSystem::GetPath("res/splines/look_at_spline01.txt").c_str()));
    this->m_camUtil.PathSetTime(0.0f);

    // meshes
    this->m_importMesh.Init(
        CFileSystem::GetPath("res/meshes/mesh01.obj").c_str());

    // metaballs
    this->m_metaBallsUtil.Init();

    m_metaBallsModel = glm::mat4(1.0f);
    m_metaBallsModel = glm::translate(m_metaBallsModel, glm::vec3(-2.0f, 1.0f, -3.0));
    m_metaBallsModel = glm::scale(m_metaBallsModel, glm::vec3(0.2f));

    CSceneUpdateArgs updArgs = CSceneUpdateArgs(0,0,0);	
    this->m_metaBallsUtil.Update(updArgs);

    // shader
    this->m_shader.Init(
        CFileSystem::GetPath("res/shaders/shadow_mapping.vs").c_str(),
        CFileSystem::GetPath("res/shaders/shadow_mapping.fs").c_str());

    this->m_shaderCubeMapReflect.Init(
        CFileSystem::GetPath("res/shaders/cubemaps.vs").c_str(),
        CFileSystem::GetPath("res/shaders/cubemaps.fs").c_str());

    this->m_simpleDepthShader.Init(
        CFileSystem::GetPath("res/shaders/shadow_mapping_depth.vs").c_str(),
        CFileSystem::GetPath("res/shaders/shadow_mapping_depth.fs").c_str());    

    // load textures
    this->m_texFloor = CUtil::LoadTextureFromFile(
        CFileSystem::GetPath("res/img/floor01.jpg").c_str());

    this->m_texRed = CUtil::LoadTextureFromFile(
        CFileSystem::GetPath("res/img/space_red.png").c_str());

    this->m_texBlue = CUtil::LoadTextureFromFile(
        CFileSystem::GetPath("res/img/space_blue.png").c_str());    

    std::vector<std::string> textureList;

    textureList.clear();
    textureList.push_back(CFileSystem::GetPath("res/img/cube01/slice_1_2.png").c_str());
    textureList.push_back(CFileSystem::GetPath("res/img/cube01/slice_1_0.png").c_str());
    textureList.push_back(CFileSystem::GetPath("res/img/cube01/slice_0_0.png").c_str());
    textureList.push_back(CFileSystem::GetPath("res/img/cube01/slice_2_0.png").c_str());
    textureList.push_back(CFileSystem::GetPath("res/img/cube01/slice_1_1.png").c_str());
    textureList.push_back(CFileSystem::GetPath("res/img/cube01/slice_1_3.png").c_str());	
    this->m_texCubeMap = CUtil::LoadCubeMapFromFile(textureList);
    
    // meshes
    this->m_planeMesh.Init();
    this->m_cubeMesh.Init();

    // Framebuffer
    this->m_frameBufferDepth.Init();
}

void CScene01::SetGlStates()
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // shader configuration
    m_shader.Use();
    m_shader.SetInt("diffuseTexture", 0);
    m_shader.SetInt("shadowMap", 1);
}

void CScene01::Render(CSceneUpdateArgs &args)
{
    // change light position over time
    float radius = 13.0f;
    m_lightPos.x = sin(args.GetCurrentFrame() / 3) * radius;
    m_lightPos.y = 18.0;    
    m_lightPos.z = cos(args.GetCurrentFrame() / 3) * radius;
    
    // 1. render depth of scene to texture (from light's perspective)
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 30.0f;

    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(m_lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    // render scene from light's point of view
    m_simpleDepthShader.Use();
    m_simpleDepthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);

    this->m_frameBufferDepth.Bind();    
    this->RenderCommon(args, m_simpleDepthShader);

    // metaballs    
    this->m_simpleDepthShader.SetMat4("model", m_metaBallsModel);
    this->m_metaBallsUtil.Render();

    this->m_frameBufferDepth.Unbind();    

    // reset viewport
    glViewport(0, 0, this->GetScreenWidth(), this->GetScreenHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2. render scene as normal using the generated depth/shadow map
    glViewport(0, 0, this->GetScreenWidth(), this->GetScreenHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shader.Use();
    glm::mat4 projection = glm::perspective(glm::radians(m_camUtil.m_zoom), (float)this->GetScreenWidth() / (float)this->GetScreenHeight(), 0.1f, 100.0f);
    glm::mat4 view = this->m_camUtil.GetViewMatrix();
    m_shader.SetMat4("projection", projection);
    m_shader.SetMat4("view", view);

    // set light uniforms
    m_shader.SetVec3("viewPos", m_camUtil.GetEyePosition());
    m_shader.SetVec3("lightPos", m_lightPos);
    m_shader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
    
    this->m_frameBufferDepth.Show();

    this->RenderCommon(args, m_shader);

    // metaballs
    this->m_shaderCubeMapReflect.Use();
    this->m_shaderCubeMapReflect.SetMat4("view", view);
    this->m_shaderCubeMapReflect.SetMat4("projection", projection);
    this->m_shaderCubeMapReflect.SetVec3("cameraPos", m_camUtil.GetEyePosition());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_texCubeMap.ID);

    this->m_shaderCubeMapReflect.SetMat4("model", this->m_metaBallsModel);
    this->m_metaBallsUtil.Render();
}

void CScene01::Update(CSceneUpdateArgs &args)
{
    this->m_metaBallsUtil.Update(args);
    m_camUtil.PathInterpolate(args.GetDeltaTime()); 
}

void CScene01::Release()
{
    this->m_metaBallsUtil.Release();
    this->m_planeMesh.Release();
    this->m_cubeMesh.Release();
    this->m_frameBufferDepth.Release();
    this->m_importMesh.Release();
}

void CScene01::RenderCommon(CSceneUpdateArgs &args, const CShader &shader)
{   
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    shader.SetMat4("model", model);
    
    this->m_texFloor.Activate();
    this->m_planeMesh.Render();
    this->m_texFloor.UnBind();
    
    // rotate one cube
    float radius = 6.0f;
    float posX = sin(args.GetCurrentFrame() / 4) * radius;
    float posZ = cos(args.GetCurrentFrame() / 4) * radius;

    // cubes
    this->m_texBlue.Activate();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(posX, 0.3f, posZ));
    model = glm::rotate(model, args.GetCurrentFrame() / 2.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.SetMat4("model", model);
    this->m_cubeMesh.Render();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-2.0f, 0.60f, 3.0));
    model = glm::rotate(model, args.GetCurrentFrame() / 2.0f, glm::normalize(glm::vec3(1.0, 1.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.SetMat4("model", model);
    this->m_cubeMesh.Render();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-posX, 2.0f, -posZ));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::rotate(model, args.GetCurrentFrame() / 2.0f, glm::normalize(glm::vec3(1.0, 0.4, 0.2)));
    model = glm::scale(model, glm::vec3(0.25));
    shader.SetMat4("model", model);
    this->m_cubeMesh.Render();
    this->m_texBlue.UnBind();

    // mesh
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0f, 2.0f, 0.0f));
    model = glm::rotate(model, args.GetCurrentFrame() / 2.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.2)));
    model = glm::scale(model, glm::vec3(2.0f));
    
    shader.SetMat4("model", model);

    this->m_texRed.Activate();
    this->m_importMesh.Render();
    this->m_texRed.UnBind();
}