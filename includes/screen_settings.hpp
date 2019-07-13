#ifndef SCREEN_SETTINGS_HPP
#define SCREEN_SETTINGS_HPP

#include <glad/glad.h>

class CScreenSettings
{
public:    
    CScreenSettings()
    {        
    }
    
    CScreenSettings(GLsizei fScreenWidth, GLsizei fScreenHeight)
    {
        this->m_fScreenWidth = fScreenWidth;
        this->m_fScreenHeight = fScreenHeight;
    }

    GLsizei GetScreenWidth()
    {
        return this->m_fScreenWidth;
    }

    GLsizei GetScreenHeight()
    {
        return this->m_fScreenHeight;
    }

private:
    GLsizei m_fScreenWidth;
    GLsizei m_fScreenHeight;
};
#endif