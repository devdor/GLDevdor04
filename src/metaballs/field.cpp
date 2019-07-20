#include "../includes/metaballs/field.hpp"
#include "../includes/metaballs/tables.hpp"

#include <float.h>
#include <glm/glm.hpp>

CField::CField()
{
  // Maximum of 5 triangles per cube and 3 points for triangle
  m_pointsMesh = new glm::vec3[FIELD_SIZE*FIELD_SIZE*FIELD_SIZE*5*3];
  m_normalesMesh = new glm::vec3[FIELD_SIZE*FIELD_SIZE*FIELD_SIZE * 5 * 3];

  nbPoints=0;
  m_cubesNonExplores.reserve(1000);
};

CField::~CField()
{
  delete[] m_pointsMesh;
  delete[] m_normalesMesh;
};

void CField::Calculate(CMetaball *liste, int numMb)
{
  int x,y,z,i;
  glm::vec3 pos;

  for (z=0;z<FIELD_SIZE;z++)
  {
      for (y=0;y<FIELD_SIZE;y++)
      {
        for (x=0;x<FIELD_SIZE;x++)
        {
          pos.x=x;
          pos.y=y;
          pos.z=z;

	        m_field[x][y][z]=0.0f;

		      for (i = 0; i < numMb; i++)
          {
            m_field[x][y][z] += liste[i].Compute(pos);
          }
        }
	    }
    }
  
    for (z=1;z<FIELD_SIZE-1;z++)
    {
      for (y=1;y<FIELD_SIZE-1;y++)
      {
        for (x=1;x<FIELD_SIZE-1;x++)
        {
	        m_gradient[x][y][z].x=m_field[x+1][y][z]-m_field[x-1][y][z];
	        m_gradient[x][y][z].y=m_field[x][y+1][z]-m_field[x][y-1][z];
	        m_gradient[x][y][z].z=m_field[x][y][z+1]-m_field[x][y][z-1];
	        m_gradient[x][y][z]*=-0.5;
        }
      }
    }
};  

void CField::AddNeighbors(int x, int y, int z)
{
  if (x+1<FIELD_SIZE)
    m_cubesNonExplores.push_back(CCubeCoord(x+1,y,z));

  if (x-1>=0)
    m_cubesNonExplores.push_back(CCubeCoord(x-1,y,z));

  if (y+1<FIELD_SIZE)
    m_cubesNonExplores.push_back(CCubeCoord(x,y+1,z));

  if (y-1>=0)
    m_cubesNonExplores.push_back(CCubeCoord(x,y-1,z));

  if (z+1<FIELD_SIZE)
    m_cubesNonExplores.push_back(CCubeCoord(x,y,z+1));

  if (z-1>=0)
    m_cubesNonExplores.push_back(CCubeCoord(x,y,z-1));
};


void CField::Triangulation(float valueReference)
{
  int x,y,z;

  nbPoints=0;

  for (x = 0; x < FIELD_SIZE - 1; x++)
  {
	  for (y = 0; y < FIELD_SIZE - 1; y++)
	  {
		  for (z = 0; z < FIELD_SIZE - 1; z++)
		  {
			  MarchingCubes(x, y, z, valueReference);
		  }
	  }
  }
};

void CField::TriangleOptimization(CMetaball *liste, int numMb, float valueReference)
{
	int x,y,z,cx,cy,cz;
	CCubeCoord last(0,0,0);
	
	nbPoints=0;
	
	for (z = 0; z < FIELD_SIZE; z++)
	{
		for (y = 0; y < FIELD_SIZE; y++)
		{
			for (x = 0; x < FIELD_SIZE; x++)
			{
				CubeDefined[x][y][z] = false;
			}
		}
	}
	
	for (int n=0;n<numMb;n++)
    {
		bool trouveSurface=false;
		
    glm::vec3 center = liste[n].GetCenter();

		cx=(int)center.x;
		cy=(int)center.y;
		cz=(int)center.z;
		
		while (!trouveSurface && cx < FIELD_SIZE)
		{
			if (CubeDefined[cx][cy][cz])
				trouveSurface=true;
			else
			{
				CubeDefined[cx][cy][cz]=true;
				if (MarchingCubes(cx,cy,cz,valueReference))
        {
          trouveSurface=true;
        }					
				else
        {
          cx++;
        }					
			}
		}
		
		AddNeighbors(cx,cy,cz);
		
		do 
		{
			last = m_cubesNonExplores.back();
			if (CubeDefined[last.x][last.y][last.z])
      {
        m_cubesNonExplores.pop_back();
      }				
			else
			{
				if (MarchingCubes(last.x,last.y,last.z,valueReference))
				{
          m_cubesNonExplores.pop_back();
					AddNeighbors(last.x,last.y,last.z);
				}
        
        CubeDefined[last.x][last.y][last.z]=true;
			}
		}
		while (!m_cubesNonExplores.empty());
  }
}

bool CField::MarchingCubes(int x,int y,int z,float valueReference)
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

  valeur[7]=m_field[x][y][z];
  valeur[4]=m_field[x][y][z+1];
  valeur[6]=m_field[x+1][y][z];
  valeur[5]=m_field[x+1][y][z+1];
  valeur[3]=m_field[x][y+1][z];
  valeur[0]=m_field[x][y+1][z+1];
  valeur[2]=m_field[x+1][y+1][z];
  valeur[1]=m_field[x+1][y+1][z+1];

  grad[7]=m_gradient[x][y][z];
  grad[4]=m_gradient[x][y][z+1];
  grad[6]=m_gradient[x+1][y][z];
  grad[5]=m_gradient[x+1][y][z+1];
  grad[3]=m_gradient[x][y+1][z];
  grad[0]=m_gradient[x][y+1][z+1];
  grad[2]=m_gradient[x+1][y+1][z];
  grad[1]=m_gradient[x+1][y+1][z+1];
    
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
      if (valeur[n]>valueReference)
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
	    rapport=(valueReference-valeur[A])/(valeur[B]-valeur[A]);
	  
	  normales[n]=grad[A]+(grad[B]-grad[A])*rapport;
	  points[n]=position[A]+(position[B]-position[A])*rapport;	  
	}
      masque<<=1;
    }
  
  n=0;
  while (tableTriangles[index][n]!=-1)
    {
      m_pointsMesh[nbPoints]=points[tableTriangles[index][n]];
      m_pointsMesh[nbPoints+1]=points[tableTriangles[index][n+1]];
      m_pointsMesh[nbPoints+2]=points[tableTriangles[index][n+2]];

      m_normalesMesh[nbPoints]=normales[tableTriangles[index][n]];
      m_normalesMesh[nbPoints+1]=normales[tableTriangles[index][n+1]];
      m_normalesMesh[nbPoints+2]=normales[tableTriangles[index][n+2]];

      nbPoints+=3;
      n+=3;
    }
  
  return true;
}

glm::vec3 CField::GetPointMesh(int nIdx)
{
  return this->m_pointsMesh[nIdx];
}

glm::vec3 CField::GetNormalMesh(int nIdx)
{
  return this->m_normalesMesh[nIdx];
}

