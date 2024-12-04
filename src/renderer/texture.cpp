#include "texture.h"

namespace peculiar
{

texture_t::texture_t(uint32_t id)
    : m_id(id)
{
}

void texture_t::bind(GLuint unit) const
{
    glBindTextureUnit(unit, m_id);
}

ref_ptr_t<texture_t> texture_t::load_white_texture()
{
    uint32_t white = 0xFFFFFFFF;

    uint32_t id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white);

    return make_ref<texture_t>(id);
}

}