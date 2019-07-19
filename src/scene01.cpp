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
        CUtil::LoadSpline(CFileSystem::GetPath("res/splines/eye_spline02.txt").c_str()),
        CUtil::LoadSpline(CFileSystem::GetPath("res/splines/look_at_spline02.txt").c_str()));
    this->m_camUtil.PathSetTime(0.0f);

    // meshes
    this->m_importMesh.Init(
        CFileSystem::GetPath("res/meshes/mesh01.obj").c_str());

    // metaballs
	this->m_champ = new CChamp();
    CSceneUpdateArgs updArgs = CSceneUpdateArgs(0,0,0);
	this->CalculateMetaballs(updArgs);
	this->InitMetaballsVertexBuffer();

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

    // configure depth map FBO    
    glGenFramebuffers(1, &this->m_depthMapFBO);

    // create depth texture    
    glGenTextures(1, &this->m_depthMap);
    glBindTexture(GL_TEXTURE_2D, this->m_depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HPPEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->m_depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // change light position over time
    m_lightPos.x = sin(args.GetCurrentFrame()) / 2.0f;
    m_lightPos.z = cos(args.GetCurrentFrame()) / 2.0f;
    m_lightPos.y = 18.0;    

    // 1. render depth of scene to texture (from light's perspective)
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 20.0f;

    //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HPPEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(m_lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    // render scene from light's point of view
    m_simpleDepthShader.Use();
    m_simpleDepthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HPPEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    this->RenderCommonObjects(args, m_simpleDepthShader);
    this->RenderMetaballs(args, m_simpleDepthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->m_depthMap);
    this->RenderCommonObjects(args, m_shader);

    // Switch to CubeMap shader
    this->m_shaderCubeMapReflect.Use();
    this->m_shaderCubeMapReflect.SetMat4("view", view);
    this->m_shaderCubeMapReflect.SetMat4("projection", projection);
    this->m_shaderCubeMapReflect.SetVec3("cameraPos", m_camUtil.GetEyePosition());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_texCubeMap.ID);
    this->RenderMetaballs(args, this->m_shaderCubeMapReflect);
}

void CScene01::Update(CSceneUpdateArgs &args)
{
    this->CalculateMetaballs(args);   
    this->UpdateMetaballsVertexBuffer();
    m_camUtil.PathInterpolate(args.GetDeltaTime()); 
}

void CScene01::Release()
{
    this->m_planeMesh.Release();
    this->m_cubeMesh.Release();
}

void CScene01::RenderCommonObjects(CSceneUpdateArgs &args, const CShader &shader)
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
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
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

void CScene01::RenderMetaballs(CSceneUpdateArgs &args, const CShader &shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
    model = glm::scale(model, glm::vec3(0.2f));
    shader.SetMat4("model", model);
    
	glBindVertexArray(this->m_vaoMetaballs);
	glDrawArrays(GL_TRIANGLES, 0, this->m_MetaballVertices.size());
	glBindVertexArray(0);
}

void CScene01::CalculateMetaballs(CSceneUpdateArgs &args)
{
	// Update Positions
	CMetaball listeMetaballs[NUM_BALLS];
	for (int n = 0; n < NUM_BALLS; n++)
	{
		listeMetaballs[n].centre.x = 16 + 8 * sin(n + args.GetCurrentFrame() * 0.3);
		listeMetaballs[n].centre.y = 16 + 8 * cos(2 * n + args.GetCurrentFrame() * 0.5);
		listeMetaballs[n].centre.z = 16 + 8 * sin(2 * n + args.GetCurrentFrame() * 0.3);
	}

	this->m_champ->Calculate((CMetaball *)&listeMetaballs, NUM_BALLS);
	this->m_champ->TriangleOptimization((CMetaball *)&listeMetaballs, NUM_BALLS, 0.2f);

	// Calculate VertexBuffer
	this->m_MetaballVertices.clear();
	this->m_MetaballVertices.reserve(this->m_champ->nbPoints);
	for (int i = 0; i < this->m_champ->nbPoints; i++)
	{
		Vertex vertex;
		vertex.Position = this->m_champ->pointsMesh[i];
		vertex.Normal = this->m_champ->normalesMesh[i];
		vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		this->m_MetaballVertices.push_back(vertex);
	}
}

void CScene01::InitMetaballsVertexBuffer()
{
	glGenVertexArrays(1, &this->m_vaoMetaballs);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, this->m_MetaballVertices.size() * sizeof(Vertex), &this->m_MetaballVertices[0], GL_STATIC_DRAW);

	glBindVertexArray(this->m_vaoMetaballs);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

void CScene01::UpdateMetaballsVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, this->m_MetaballVertices.size() * sizeof(Vertex), &this->m_MetaballVertices[0], GL_STATIC_DRAW);
}