#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "screen_settings.hpp"

class CCamera
{

public:
    CCamera();
    CCamera(const CScreenSettings &screenSettings);
    ~CCamera();

    glm::mat4 GetViewMatrix();
    glm::vec3 GetEyePosition();
    glm::vec3 GetLookAtPosition();
    glm::mat4 GetProjectionMatrix();

    void Release();
    void UpdateEyePosition(glm::vec3 vec);
    void UpdateEyePosition(float x, float y, float z);
    void UpdateLookAtPos(glm::vec3 vec);
    void UpdateLookAtPos(float x, float y, float z);
    void UpdateZoom(float value);
    void UpdateUpVector(glm::vec3 value);
    void InitView(const CScreenSettings &screenSettings);

    GLsizei GetScreenWidth()
    {
        return this->m_screenSettings.GetScreenWidth();
    }

    GLsizei GetScreenHeight()
    {
        return this->m_screenSettings.GetScreenHeight();
    }

    GLfloat m_zoom = 45.0f;
protected:
    glm::vec3 m_eyePosition;
    glm::vec3 m_lookatPosition;
    glm::vec3 m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);

private:
    CScreenSettings m_screenSettings;
};
#endif