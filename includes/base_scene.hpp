#ifndef BASE_SCENE_HPP
#define BASE_SCENE_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
#include "file_system.hpp"
#include "camera_util.hpp"
#include "scene_init_args.hpp"
#include "scene_update_args.hpp"
#include "screen_settings.hpp"

class CBaseScene
{
public:
    CBaseScene();
    ~CBaseScene();

    void InitView(const CScreenSettings &screenSettings);
    GLsizei GetScreenWidth()
    {
        return this->m_camUtil.GetScreenWidth();
    }

    GLsizei GetScreenHeight()
    {
        return this->m_camUtil.GetScreenHeight();
    }
protected:
    CCameraUtil m_camUtil;
};
#endif