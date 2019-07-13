#include "util.hpp"
#include "file_system.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

std::vector<glm::vec3> CUtil::LoadSpline(const std::string& filename)
{

    if (!CFileSystem::FileExists(filename.c_str()))
    {
        std::stringstream msg;
        msg << "Spline '" << filename << "' not found!" << std::endl;

        throw std::invalid_argument(msg.str().c_str());
    }

    std::ifstream fp(filename);

    std::string str;
    std::string strBuffer;
    std::getline(fp, strBuffer);
    if (strBuffer.size() > 3)
    {
        std::stringstream strStream(strBuffer);
        float f;
        strStream >> str >> f;
    }

    std::vector<glm::vec3> items;
    while (std::getline(fp, strBuffer))
    {
        if (strBuffer.size() > 3)
        {
            std::stringstream strStream(strBuffer);

            glm::vec3 point;
            strStream >> str >> point.x >> point.y >> point.z;
            items.push_back(point);
        }
    }

    return items;
}

CTexture2D CUtil::LoadTextureFromFile(const std::string& filename)
{
	if (!CFileSystem::FileExists(filename.c_str()))
    {
        std::stringstream msg;
        msg << "Spline '" << filename << "' not found!" << std::endl;

        throw std::invalid_argument(msg.str().c_str());
    }

    // Create Texture object
	CTexture2D tex2d;

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        tex2d.GenerateFromFile(width, height, format, data);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture '" << filename << "'" << std::endl;
        stbi_image_free(data);
    }
    
	return tex2d;
}

CTextureCubeMap CUtil::LoadCubeMapFromFile(const std::vector<std::string> files)
{
	CTextureCubeMap texture;

	int width, height, nrComponents;        
    std::vector<unsigned char*> imageData;
    
	for (int i = 0; i < files.size(); i++)
	{
        if (!CFileSystem::FileExists(files[i]))
        {
            std::stringstream msg;
            msg << "Spline '" << files[i] << "' not found!" << std::endl;

            throw std::invalid_argument(msg.str().c_str());
        }
        
        unsigned char *data = stbi_load(files[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            imageData.push_back(data);
        }		
	}

    GLenum format;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;

	texture.GenerateTex(width, height, format, imageData);
	for (int i = 0; i < imageData.size(); i++)
	{
        stbi_image_free(imageData[i]);
	}
	return texture;
}