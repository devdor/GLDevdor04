#ifndef Field_HPP
#define Field_HPP

#include <vector>
#include <glm/glm.hpp>
#include "cubecoord.hpp"
#include "metaball.hpp"

#define FIELD_SIZE 32

class CField
{
 public:
  int nbPoints;  

  CField();
  ~CField();
  
  void Calculate(CMetaball *liste, int numMb);
  void Triangulation(float valueReference);
  void TriangleOptimization(CMetaball *liste, int numMb, float valueReference);

  glm::vec3 GetPointMesh(int nIdx);
  glm::vec3 GetNormalMesh(int nIdx);

  private:
  glm::vec3 *m_pointsMesh;
  glm::vec3 *m_normalesMesh;

  float m_field[FIELD_SIZE][FIELD_SIZE][FIELD_SIZE];
  glm::vec3 m_gradient[FIELD_SIZE][FIELD_SIZE][FIELD_SIZE];  
  std::vector<CCubeCoord> m_cubesNonExplores;
  
  bool CubeDefined[FIELD_SIZE][FIELD_SIZE][FIELD_SIZE];
  bool MarchingCubes(int x, int y, int z, float valueReference);
  void AddNeighbors(int x, int y, int z);
};
#endif