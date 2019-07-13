#ifndef CUBE_COORD_HPP
#define CUBE_COORD_HPP

class CCubeCoord
{
public:
	int x, y, z;
	CCubeCoord()
	{
	}

	CCubeCoord(int X, int Y, int Z)
		:x(X), y(Y), z(Z)
	{
	}
};
#endif