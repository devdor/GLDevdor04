#include "../includes/base_scene.hpp"

CBaseScene::CBaseScene()
{     
}

CBaseScene::~CBaseScene()
{
}

void CBaseScene::InitView(const CScreenSettings &screenSettings)
{
    this->m_camUtil.InitView(screenSettings);
}

void CBaseScene::Init(CSceneInitArgs &args)
{    
}

void CBaseScene::SetGlStates()
{    
}

void CBaseScene::Render(CSceneUpdateArgs &args)
{    
}

void CBaseScene::Update(CSceneUpdateArgs &args)
{    
}

void CBaseScene::Release()
{    
}