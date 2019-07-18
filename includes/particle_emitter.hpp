#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include <glm/glm.hpp>

#include <vector>
#include <memory>

using namespace std;

struct CParticle {

	int type;								//used only for "fire with smoke" to tell between fire and smoke 0=fire, 1=smoke, 2=other
	float lifespan, age, scale, direction;	// how long the particle will exist for, alpha blending variable; how old is it.
	glm::vec3 position;						// initial onscreen position
	glm::vec3 rotation;
	glm::vec3 movement;						// movement vector
	glm::vec3 color;						// color values
	glm::vec3 pull;							// compounding directional pull in the x,y,z directions
	glm::vec2 texCoords;

	CParticle() : 
		position(0.0f), movement(0.0f), color(1.0f), pull(0.0f) 
	{ 
	}
};


class CParticleEmitter
{
public:
	CParticleEmitter();
	
	// initializes the particle system and its instanced render data
	void Init(int maxParticles);

	// updates all particles' state
	void UpdateParticles();

	vector<CParticle*> m_Particles;
	int m_maxParticles;

private:

	void CreateParticle(CParticle *p);
	void CreateParticles(void);
};
#endif