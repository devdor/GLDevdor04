#ifndef SCENE_UPDATE_ARGS_HPP
#define SCENE_UPDATE_ARGS_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/glm.hpp>

class CSceneUpdateArgs
{
public:
    CSceneUpdateArgs()
    {        
    }

    CSceneUpdateArgs(float currentFrame, float deltaTime, float lastFrame)
    {
        this->m_currentFrame = currentFrame;
        this->m_deltaTime = deltaTime;
        this->m_lastFrame = lastFrame;
    }

    float GetCurrentFrame()
    {
        return this->m_currentFrame;
    }

    float GetDeltaTime()
    {
        return this->m_deltaTime;
    }

    float GetLastFrame()
    {
        return this->m_lastFrame;
    }
private:
    float m_currentFrame = 0.0f;
    float m_deltaTime = 0.0f;
    float m_lastFrame = 0.0f;    
};
#endif