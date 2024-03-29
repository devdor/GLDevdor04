#ifndef SPLINEGL_HPP
#define SPLINEGL_HPP

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

class Interpol3D;

class SplineGL
{
public:
    SplineGL();
    ~SplineGL()
    {
        Destroy();
    }
    void Destroy();


    inline void setSpeed(float speed)
    {
        m_fSpeed = speed;
    }
    bool isValid() const;

    glm::vec3 getPoint();
    bool AddPoint(const glm::vec3& pt);
    bool DeleteLastPoint();
    bool Clear();

    void BuildSplines(bool closed, float speed = 0.01f);

    void Idle(float fElapsedTime);
    void DrawGL();

    const std::vector<glm::vec3>&	getControlPoints()	const;

    inline bool		isFinished()		const
    {
        return m_bFinished;
    }
    inline float	getSpeed()			const
    {
        return m_fSpeed;
    }
    inline void 	setTime(float t)
    {
        m_t = t;
    }
    inline float	getTime()			const
    {
        return m_t;
    }

private:
    Interpol3D *	m_pSpline;
    float			m_t;
    float			m_fSpeed;
    bool			m_bFinished;
};
#endif