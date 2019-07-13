#ifndef INTERPOL_HPP
#define INTERPOL_HPP

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class CInterpol
{
protected:
    std::vector<glm::vec2> ctrlpoints;	//tableau des points de contr�le

public:
    CInterpol();
    CInterpol(const std::vector<glm::vec2>& controlpoints);
    virtual ~CInterpol();

    virtual glm::vec2 getpoint(float t) = 0;

};
#endif