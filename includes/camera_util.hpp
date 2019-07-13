#ifndef CAMERA_UTIL_HPP
#define CAMERA_UTIL_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.hpp"
#include "spline3d/splinegl.hpp"
#include "screen_settings.hpp"

class CCameraUtil : public CCamera
{

public:
    CCameraUtil();
    CCameraUtil(const CScreenSettings &screenSettings);
    ~CCameraUtil();
    
    void Release();
    void PathInit(GLfloat fSpeed, std::vector<glm::vec3> eyePath, std::vector<glm::vec3> lookAtPath);
    void PathInterpolate(GLfloat fElapsedTime);
    void PathSetTime(GLfloat fTime);
private:

    SplineGL* m_pCamEyeTraj;
    SplineGL* m_pCamLookAtTraj;
};
#endif