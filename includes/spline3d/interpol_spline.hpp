#ifndef INTERPOLSPLINE_HPP
#define INTERPOLSPLINE_HPP

#include <glm/glm.hpp>
#include "interpol.hpp"

class CInterpolSpline : public CInterpol
{
public:
    CInterpolSpline(const std::vector<glm::vec2>& controlpoints);

    void calculderivsecondes(float yp0, float ypnm1);
    glm::vec2 getpoint(float t);
    virtual ~CInterpolSpline();
private:
    std::vector<float> dersec;
};
#endif