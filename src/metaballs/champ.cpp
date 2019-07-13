#include "../includes/metaballs/champ.hpp"
#include "../includes/metaballs/tables.hpp"

#include <float.h>

// GLM Mathemtics
#include <glm/glm.hpp>

CChamp::CChamp()
{
  // Maximum of 5 triangles per cube and 3 points for triangle
  pointsMesh = new glm::vec3[TAILLE_CHAMP*TAILLE_CHAMP*TAILLE_CHAMP*5*3];
  normalesMesh = new glm::vec3[TAILLE_CHAMP*TAILLE_CHAMP*TAILLE_CHAMP * 5 * 3];

  nbPoints=0;
  cubesNonExplores.reserve(1000);
};

CChamp::~CChamp()
{
  delete[] pointsMesh;
  delete[] normalesMesh;
};

void CChamp::Calculate(CMetaball *liste, int nombre_mb)
{
  int x,y,z,i;
  glm::vec3 pos;

  for (z=0;z<TAILLE_CHAMP;z++)
  {
      for (y=0;y<TAILLE_CHAMP;y++)
      {
        for (x=0;x<TAILLE_CHAMP;x++)
        {
          pos.x=x;
          pos.y=y;
          pos.z=z;

	        champ[x][y][z]=0.0f;

		      for (i = 0; i < nombre_mb; i++)
          {
            champ[x][y][z] += liste[i].fonction(pos);
          }
        }
	    }
    }
  
    for (z=1;z<TAILLE_CHAMP-1;z++)
    {
      for (y=1;y<TAILLE_CHAMP-1;y++)
      {
        for (x=1;x<TAILLE_CHAMP-1;x++)
        {
	        gradient[x][y][z].x=champ[x+1][y][z]-champ[x-1][y][z];
	        gradient[x][y][z].y=champ[x][y+1][z]-champ[x][y-1][z];
	        gradient[x][y][z].z=champ[x][y][z+1]-champ[x][y][z-1];
	        gradient[x][y][z]*=-0.5;
        }
      }
    }
};  

void CChamp::AjouteVoisins(int x, int y, int z)
{
  if (x+1<TAILLE_CHAMP)
    cubesNonExplores.push_back(CCubeCoord(x+1,y,z));
  if (x-1>=0)
    cubesNonExplores.push_back(CCubeCoord(x-1,y,z));

  if (y+1<TAILLE_CHAMP)
    cubesNonExplores.push_back(CCubeCoord(x,y+1,z));
  if (y-1>=0)
    cubesNonExplores.push_back(CCubeCoord(x,y-1,z));

  if (z+1<TAILLE_CHAMP)
    cubesNonExplores.push_back(CCubeCoord(x,y,z+1));
  if (z-1>=0)
    cubesNonExplores.push_back(CCubeCoord(x,y,z-1));
};


void CChamp::Triangulation(float valeurReference)
{
  int x,y,z;

  nbPoints=0;

  for (x = 0; x < TAILLE_CHAMP - 1; x++)
  {
	  for (y = 0; y < TAILLE_CHAMP - 1; y++)
	  {
		  for (z = 0; z < TAILLE_CHAMP - 1; z++)
		  {
			  MarchingCubes(x, y, z, valeurReference);
		  }
	  }
  }
};

void CChamp::TriangleOptimization(CMetaball *liste, int nombre_mb, float valeurReference)
{
	int x,y,z,cx,cy,cz;
	CCubeCoord last(0,0,0);
	
	nbPoints=0;
	
	for (z = 0; z < TAILLE_CHAMP; z++)
	{
		for (y = 0; y < TAILLE_CHAMP; y++)
		{
			for (x = 0; x < TAILLE_CHAMP; x++)
			{
				CubeTraite[x][y][z] = false;
			}
		}
	}
	
	for (int n=0;n<nombre_mb;n++)
    {
		bool trouveSurface=false;
		
		cx=(int)liste[n].centre.x;
		cy=(int)liste[n].centre.y;
		cz=(int)liste[n].centre.z;
		
		while (!trouveSurface && cx < TAILLE_CHAMP)
		{
			if (CubeTraite[cx][cy][cz])
				trouveSurface=true;
			else
			{
				CubeTraite[cx][cy][cz]=true;
				if (MarchingCubes(cx,cy,cz,valeurReference))
					trouveSurface=true;
				else
					cx++;
			}
		}
		
		AjouteVoisins(cx,cy,cz);
		
		do 
		{
			last = cubesNonExplores.back();
			if (CubeTraite[last.x][last.y][last.z])
				cubesNonExplores.pop_back();
			else
			{
				if (MarchingCubes(last.x,last.y,last.z,valeurReference))
				{
					cubesNonExplores.pop_back();
					AjouteVoisins(last.x,last.y,last.z);
				}
				
				CubeTraite[last.x][last.y][last.z]=true;
			}
		}
		while (!cubesNonExplores.empty());
  }
}

bool CChamp::MarchingCubes(int x,int y,int z,float refValue)
{
  static int n,index,masque;
  static int aretes,A,B;
  static float rapport;

  const static unsigned int sommetDebut[12]={0,1,2,3,4,5,6,7,0,1,2,3};
  const static unsigned int sommetFin[12]={1,2,3,0,5,6,7,4,4,5,6,7};

  static float valeur[8];

  static glm::vec3 position[8];
  static glm::vec3 grad[8];

  static glm::vec3 points[12];
  static glm::vec3 normales[12];

  valeur[7]=champ[x][y][z];
  valeur[4]=champ[x][y][z+1];
  valeur[6]=champ[x+1][y][z];
  valeur[5]=champ[x+1][y][z+1];
  valeur[3]=champ[x][y+1][z];
  valeur[0]=champ[x][y+1][z+1];
  valeur[2]=champ[x+1][y+1][z];
  valeur[1]=champ[x+1][y+1][z+1];

  grad[7]=gradient[x][y][z];
  grad[4]=gradient[x][y][z+1];
  grad[6]=gradient[x+1][y][z];
  grad[5]=gradient[x+1][y][z+1];
  grad[3]=gradient[x][y+1][z];
  grad[0]=gradient[x][y+1][z+1];
  grad[2]=gradient[x+1][y+1][z];
  grad[1]=gradient[x+1][y+1][z+1];
    
  position[7].x = x;
  position[7].y = y;
  position[7].z = z;

  position[4].x = x;
  position[4].y = y;
  position[4].z = z + 1;

  position[6].x = x + 1;
  position[6].y = y;
  position[6].z = z;

  position[5].x = x + 1;
  position[5].y = y;
  position[5].z = z + 1;

  position[3].x = x;
  position[3].y = y + 1;
  position[3].z = z;

  position[0].x = x;
  position[0].y = y + 1;
  position[0].z = z + 1;

  position[2].x = x + 1;
  position[2].y = y + 1;
  position[2].z = z;

  position[1].x = x + 1;
  position[1].y = y + 1;
  position[1].z = z + 1;

  masque=1;
  index=0;

  for (n=0;n<8;n++)
    {
      if (valeur[n]>refValue)
	index|=masque;
      masque<<=1;
    }
  
  if (index==0 || index==255)
    return false;

  aretes=tableEdges[index];

  for (int n=0,masque=1;n<12;n++)
    {
      A=sommetDebut[n];
      B=sommetFin[n];

      if (aretes & masque) 
	{
	  if (valeur[B]==valeur[A]) 
	    rapport=0.0f;
	  else
	    rapport=(refValue-valeur[A])/(valeur[B]-valeur[A]);
	  
	  normales[n]=grad[A]+(grad[B]-grad[A])*rapport;
	  points[n]=position[A]+(position[B]-position[A])*rapport;	  
	}
      masque<<=1;
    }
  
  n=0;
  while (tableTriangles[index][n]!=-1)
    {
      pointsMesh[nbPoints]=points[tableTriangles[index][n]];
      pointsMesh[nbPoints+1]=points[tableTriangles[index][n+1]];
      pointsMesh[nbPoints+2]=points[tableTriangles[index][n+2]];

      normalesMesh[nbPoints]=normales[tableTriangles[index][n]];
      normalesMesh[nbPoints+1]=normales[tableTriangles[index][n+1]];
      normalesMesh[nbPoints+2]=normales[tableTriangles[index][n+2]];

      nbPoints+=3;
      n+=3;
    }
  
  return true;
};

