// GLM Mathemtics
#include <glm/glm.hpp>
#include "../includes/metaballs/field.hpp"
#include "../includes/metaballs/mathlib.hpp"

CMetaball::CMetaball()
{
};

CMetaball::~CMetaball()
{
};

float CMetaball::Compute(glm::vec3 pos)
{
	return 1.0 / (1.0 + CMathLib::CarreNorme(pos - this->m_vCenter));
};

glm::vec3 CMetaball::GetCenter()
{
	return this->m_vCenter;
}

void CMetaball::UpdateCenter(float x, float y, float z)
{
	this->m_vCenter.x = x;
	this->m_vCenter.y = y;
	this->m_vCenter.z = z;
}