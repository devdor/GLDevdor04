#ifndef CHAMP_HPP
#define CHAMP_HPP

#include <vector>
#include <glm/glm.hpp>
#include "cubecoord.hpp"
#include "metaball.hpp"

#define TAILLE_CHAMP 32

class CChamp
{
 public:
  int nbPoints;
  glm::vec3 *pointsMesh;
  glm::vec3 *normalesMesh;

  CChamp();
  ~CChamp();
  
  void Calculate(CMetaball *liste, int nombre_mb);
  void Triangulation(float valeurReference);
  void TriangleOptimization(CMetaball *liste, int nombre_mb, float valeurReference);

 private:
  float champ[TAILLE_CHAMP][TAILLE_CHAMP][TAILLE_CHAMP];
  glm::vec3 gradient[TAILLE_CHAMP][TAILLE_CHAMP][TAILLE_CHAMP];
  bool CubeTraite[TAILLE_CHAMP][TAILLE_CHAMP][TAILLE_CHAMP];
  std::vector<CCubeCoord> cubesNonExplores;
  
  bool MarchingCubes(int x, int y, int z, float valeurReference);
  void AjouteVoisins(int x, int y, int z);
};
#endif