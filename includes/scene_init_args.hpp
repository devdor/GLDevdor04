#ifndef SCENE_INIT_ARGS_HPP
#define SCENE_INIT_ARGS_HPP

#include "screen_settings.hpp"

class CSceneInitArgs
{
public:
    CSceneInitArgs();

    CSceneInitArgs(const CScreenSettings &screenSettings)
    {
        this->m_screenSettings = screenSettings;
    }

    CScreenSettings GetScreenSettings()
    {
        return this->m_screenSettings;
    }

private:

    CScreenSettings m_screenSettings;
};
#endif