#ifndef TEXT_LAYER_HPP
#define TEXT_LAYER_HPP

#include <iostream>
#include <map>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "file_system.hpp"
#include "screen_settings.hpp"
#include "scene_init_args.hpp"
#include "scene_update_args.hpp"

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

class CTextLayer
{
public:
    CTextLayer();
    ~CTextLayer();

    struct Character {
        GLuint TextureID;   // ID handle of the glyph texture
        glm::ivec2 Size;    // Size of glyph
        glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
        GLuint Advance;     // Horizontal offset to advance to next glyph
    };

    void Init(CSceneInitArgs &args);
    void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
private:
    CShader m_shader;
    GLuint m_texture;

    GLuint m_vAO, m_vBO;
    std::map<GLchar, Character> m_characters;
};
#endif