#include "../includes/scene02.hpp"
#include "stb_image.hpp"
#include "wave_func.hpp"

CScene02::CScene02()
    :CBaseScene()
{     
}

CScene02::~CScene02()
{    
}

void CScene02::Init(CSceneInitArgs &args)
{
    // camera
    this->m_camUtil.InitView(args.GetScreenSettings());
    this->m_camUtil.PathInit(0.015f,
        CUtil::LoadSpline(CFileSystem::GetPath("res/splines/eye_spline02.txt").c_str()),
        CUtil::LoadSpline(CFileSystem::GetPath("res/splines/look_at_spline02.txt").c_str()));
    this->m_camUtil.PathSetTime(0.0f);

    // meshes
    this->m_importMesh1.Init(
        CFileSystem::GetPath("res/meshes/torus01.obj").c_str());

    this->m_importMesh2.Init(
        CFileSystem::GetPath("res/meshes/torus01.obj").c_str());

    CSceneUpdateArgs updArgs = CSceneUpdateArgs(0,0,0);

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
    this->m_texRed = CUtil::LoadTextureFromFile(
        CFileSystem::GetPath("res/img/space_red.png").c_str());

    this->m_texFloor = CUtil::LoadTextureFromFile(
        CFileSystem::GetPath("res/img/floor01.jpg").c_str());

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

    // Framebuffer
    this->m_frameBufferDepth.Init();
}

void CScene02::SetGlStates()
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // shader configuration
    m_shader.Use();
    m_shader.SetInt("diffuseTexture", 0);
    m_shader.SetInt("shadowMap", 1);
}

void CScene02::Render(CSceneUpdateArgs &args)
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

    // torus
    glm::mat4 torusModel;
    torusModel = glm::mat4(1.0f);
    torusModel = glm::translate(torusModel, glm::vec3(0.0f, 2.0f, 0.0f));
    torusModel = glm::rotate(torusModel, args.GetCurrentFrame() / 2.0f, glm::normalize(glm::vec3(1.0, 1.0, 0.2)));
    torusModel = glm::scale(torusModel, glm::vec3(1.5f));

    this->m_simpleDepthShader.SetMat4("model", torusModel);
    this->m_importMesh1.Render();

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

    this->m_shaderCubeMapReflect.SetMat4("model", torusModel);
    this->m_importMesh1.Render();    
}

void CScene02::Update(CSceneUpdateArgs &args)
{
    this->WaveFuncObject(this->m_importMesh2, args.GetCurrentFrame());
    m_camUtil.PathInterpolate(args.GetDeltaTime()); 
}

void CScene02::Release()
{
    this->m_planeMesh.Release();
    this->m_frameBufferDepth.Release();
    this->m_importMesh1.Release();
    this->m_importMesh2.Release();
}

void CScene02::RenderCommon(CSceneUpdateArgs &args, const CShader &shader)
{   
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    shader.SetMat4("model", model);
    
    this->m_texFloor.Activate();
    this->m_planeMesh.Render();
    this->m_texFloor.UnBind();

    // rotate one mesh
    float radius = 4.0f;
    float posX = sin(args.GetCurrentFrame() / -4) * radius;
    float posZ = cos(args.GetCurrentFrame() / -4) * radius;

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(posX, 2.0f, posZ));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::rotate(model, args.GetCurrentFrame() / 2.0f, glm::normalize(glm::vec3(1.0, 0.4, 0.2)));
    model = glm::scale(model, glm::vec3(0.75f));
    shader.SetMat4("model", model);

    this->m_texRed.Activate();
    this->m_importMesh2.Render();
    this->m_texRed.UnBind();
}

void CScene02::WaveFuncObject(CImportMesh &mesh, const float curFrame)
{
    CWaveFunc wave;
	wave.func = FUNC_SIN;   // sine wave function
	wave.amp = 0.002f;      // amplitude
	wave.freq = 0.3f;       // cycles/sec
	wave.phase = 0.1f;      // horizontal shift
	wave.offset = 0.0f;     // vertical shift

	float waveLength = 1.8f;

	int numVertices = mesh.vertices.size();
	for (size_t n = 0; n < numVertices; n++)
    {
		Vertex srcVertex = mesh.GetVertex(n);

		// compute phase (horizontal shift)
		wave.phase = (srcVertex.Position.x + srcVertex.Position.y + srcVertex.Position.z) / waveLength;
		float height = wave.Update(curFrame);

		glm::vec3 tmp = glm::vec3();
		tmp.x = srcVertex.Position.x + (height * srcVertex.Normal.x);
		tmp.y = srcVertex.Position.y + (height * srcVertex.Normal.y);
		tmp.z = srcVertex.Position.z + (height * srcVertex.Normal.z);
		mesh.UpdateVertex(n, tmp);
	}

	mesh.UpdateMesh();
}
