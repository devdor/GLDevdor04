#ifndef INTERPOLNONFONCT_HPP
#define INTERPOLNONFONCT_HPP

#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include "interpol_spline.hpp"

class CInterpolNonFonct
{
public:
    CInterpolNonFonct(const std::vector<glm::vec2>& controlpoints);
    virtual ~CInterpolNonFonct();

    CInterpolSpline * Splineya;
    CInterpolSpline * Splineyb;

    glm::vec2 getpoint(float t);
};
#endif