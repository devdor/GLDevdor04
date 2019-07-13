#ifndef METABALL_HPP
#define METABALL_HPP

#include <glm/glm.hpp>

class CMetaball
{
 public:
  // constructeur
  CMetaball();
  ~CMetaball();

  glm::vec3 centre;

  float fonction(glm::vec3 pos);  
};
#endif