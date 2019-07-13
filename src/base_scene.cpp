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