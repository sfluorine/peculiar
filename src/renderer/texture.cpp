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

ref_ptr_t<texture_t> texture_t::load_from_file(const fs::path& path)
{
    auto string_path = path.string();

    if (!fs::exists(path)) {
        spdlog::error("failed to load texture: file '{}' not found!",
                      string_path);
        return nullptr;
    }

    stbi_set_flip_vertically_on_load(true);

    int32_t width, height, nr_channels;
    uint8_t* pixels = stbi_load(
        string_path.c_str(), &width, &height, &nr_channels, STBI_rgb_alpha);

    uint32_t id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA8,
                 width,
                 height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 pixels);

    stbi_image_free(pixels);

    return make_ref<texture_t>(id);
}

}