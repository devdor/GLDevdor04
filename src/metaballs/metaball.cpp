// GLM Mathemtics
#include <glm/glm.hpp>
#include "../includes/metaballs/metaball.hpp"
#include "../includes/metaballs/mathlib.hpp"

CMetaball::CMetaball()
{
};

float CMetaball::fonction(glm::vec3 pos)
{
	return 1.0 / (1.0 + CMathLib::CarreNorme(pos - centre));
};

CMetaball::~CMetaball()
{
};
