#include "camera_util.hpp"

CCameraUtil::CCameraUtil()
{
}

CCameraUtil::CCameraUtil(const CScreenSettings &screenSettings)
    : CCamera(screenSettings)
{
}

CCameraUtil::~CCameraUtil()
{
}

void CCameraUtil::Release()
{
}

void CCameraUtil::PathInit(GLfloat fSpeed, std::vector<glm::vec3> eyePath, std::vector<glm::vec3> lookAtPath)
{
    // Spline
    this->m_pCamEyeTraj = new SplineGL();
    this->m_pCamLookAtTraj = new SplineGL();


    assert(m_pCamEyeTraj);
    assert(m_pCamLookAtTraj);

    m_pCamEyeTraj->Clear();
    m_pCamLookAtTraj->Clear();

    for (int i = 0; i < eyePath.size(); i++)
    {
        this->m_pCamEyeTraj->AddPoint(eyePath[i]);
        this->m_pCamLookAtTraj->AddPoint(lookAtPath[i]);
    }

    m_pCamEyeTraj->BuildSplines(false, fSpeed);
    m_pCamLookAtTraj->BuildSplines(false, fSpeed);
}

void CCameraUtil::PathSetTime(GLfloat fTime)
{
    if (this->m_pCamEyeTraj == NULL
            || this->m_pCamLookAtTraj == NULL)
        return;

    if (m_pCamEyeTraj->isValid()
            && m_pCamLookAtTraj->isValid())
    {
        this->m_pCamEyeTraj->setTime(fTime);
        this->m_pCamLookAtTraj->setTime(fTime);
    }
}

void CCameraUtil::PathInterpolate(GLfloat fElapsedTime)
{
    if (this->m_pCamEyeTraj == NULL
            || this->m_pCamLookAtTraj == NULL)
        return;

    if (m_pCamEyeTraj->isValid()
            && m_pCamLookAtTraj->isValid())
    {
        m_pCamEyeTraj->Idle(fElapsedTime);
        this->UpdateEyePosition(m_pCamEyeTraj->getPoint());

        m_pCamLookAtTraj->Idle(fElapsedTime);
        this->UpdateLookAtPos(m_pCamLookAtTraj->getPoint());
    }
}