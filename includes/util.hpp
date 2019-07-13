#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "texture2d.hpp"
#include "texture_cube_map.hpp"

class CUtil
{
public:
    static std::vector<glm::vec3> LoadSpline(const std::string& filename);
    static CTexture2D LoadTextureFromFile(const std::string& filename);
    static CTextureCubeMap LoadCubeMapFromFile(const std::vector<std::string> files);
};
#endif