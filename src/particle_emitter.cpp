#include "../includes/particle_emitter.hpp"
#include <random>

CParticleEmitter::CParticleEmitter() 
	: m_maxParticles(100)
{
}

void CParticleEmitter::Init(int maxParticles)
{
	this->m_maxParticles = maxParticles;
	this->CreateParticles();
}

// Initalizes a single particle according to its type
void CParticleEmitter::CreateParticle(CParticle *p)
{
	p->lifespan = (((rand() % 125 + 1) / 10.0f) + 5);
	p->type = 2;

	p->age = 0.0f;
	p->scale = 0.25f;
	p->direction = 0;
		
	p->position = glm::vec3(0.0f, 0.0f, 0.0f);

	p->movement.x = (((((((2) * rand() % 11) + 1)) * rand() % 11) + 1) * 0.035) - (((((((2) * rand() % 11) + 1)) * rand() % 11) + 1) * 0.035);
	p->movement.y = ((((((5) * rand() % 11) + 3)) * rand() % 11) + 7) * 0.015;
	p->movement.z = (((((((2) * rand() % 11) + 1)) * rand() % 11) + 1) * 0.015) - (((((((2) * rand() % 11) + 1)) * rand() % 11) + 1) * 0.015);

	p->movement.x = p->movement.x / 2;
	p->movement.y = p->movement.y / 2;
	p->movement.z = p->movement.z / 2;

	// rotation
	p->rotation.x = (double)rand() / (RAND_MAX + 1.0);
	p->rotation.y = (double)rand() / (RAND_MAX + 1.0);
	p->rotation.z = (double)rand() / (RAND_MAX + 1.0);	

	p->pull.x = 0.0f;
	p->pull.y = 0.0f;
	p->pull.z = 0.0f;
}

void CParticleEmitter::CreateParticles(void)
{
	for (int i=0; i < this->m_maxParticles; i++)
	{
		CParticle* p = new CParticle();
		this->CreateParticle(p);

		this->m_Particles.push_back(p);
	}
}

void CParticleEmitter::UpdateParticles()
{
	int i = 0;

	for (std::vector<CParticle*>::iterator it = this->m_Particles.begin(); it != this->m_Particles.end(); ++it)
	{
		CParticle* p = (*it);

		p->age = p->age + 0.015;
		
		p->direction = p->direction + ((((((int)(0.5) * rand() % 11) + 1)) * rand() % 11) + 1);		

		p->position.x = p->position.x + p->movement.x / 2 + p->pull.x / 2;
		p->position.y = p->position.y + p->movement.y / 2 + p->pull.y / 2;
		p->position.z = p->position.z + p->movement.z / 2 + p->pull.z / 2;
				
		if (p->age > p->lifespan || p->position.y > 19 || p->position.y < -15 || p->position.x > 18 || p->position.x < -18)
				this->CreateParticle(p);
	}
}