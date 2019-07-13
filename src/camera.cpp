#include "../includes/camera.hpp"
#include "../includes/screen_settings.hpp"

CCamera::CCamera()
{
}

CCamera::CCamera(const CScreenSettings &screenSettings)
{    
    this->InitView(screenSettings);
}

void CCamera::InitView(const CScreenSettings &screenSettings)
{
    this->m_screenSettings = screenSettings;
}

CCamera::~CCamera()
{
}

void CCamera::Release()
{
}

glm::mat4 CCamera::GetViewMatrix()
{
    return glm::lookAt(m_eyePosition, m_lookatPosition, m_upVector);
}

glm::mat4 CCamera::GetProjectionMatrix()
{
    return glm::perspective(this->m_zoom, (GLfloat)this->GetScreenWidth() / (GLfloat)this->GetScreenHeight(), 0.1f, 350.0f);
}

glm::vec3 CCamera::GetEyePosition()
{
    return this->m_eyePosition;
}

glm::vec3 CCamera::GetLookAtPosition()
{
    return this->m_lookatPosition;
}

void CCamera::UpdateZoom(float value)
{
    this->m_zoom = value;
}

void CCamera::UpdateEyePosition(glm::vec3 vec)
{
    this->UpdateEyePosition(vec.x, vec.y, vec.z);
}

void CCamera::UpdateEyePosition(float x, float y, float z)
{
    this->m_eyePosition.x = x;
    this->m_eyePosition.y = y;
    this->m_eyePosition.z = z;
}

void CCamera::UpdateLookAtPos(glm::vec3 vec)
{
    this->UpdateLookAtPos(vec.x, vec.y, vec.z);
}

void CCamera::UpdateLookAtPos(float x, float y, float z)
{
    this->m_lookatPosition.x = x;
    this->m_lookatPosition.y = y;
    this->m_lookatPosition.z = z;
}

void CCamera::UpdateUpVector(glm::vec3 value)
{
    this->m_upVector = value;
}