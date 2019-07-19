#ifndef METABALL_HPP
#define METABALL_HPP

#include <glm/glm.hpp>

class CMetaball
{
  public:
  
    CMetaball();
    ~CMetaball();
  
    float Compute(glm::vec3 pos);
    glm::vec3 GetCenter();
    void UpdateCenter(float x, float y, float z);

  private:
    glm::vec3 m_vCenter;
};
#endif