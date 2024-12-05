#pragma once

#include "common.h"

namespace peculiar
{

class texture_t {
public:
    texture_t(uint32_t id);

    texture_t(const texture_t& id) = delete;
    texture_t& operator=(const texture_t& id) = delete;

    texture_t(texture_t&& id) = delete;
    texture_t& operator=(texture_t&& id) = delete;

    void bind(GLuint unit) const;

public:
    static ref_ptr_t<texture_t> load_white_texture();
    static ref_ptr_t<texture_t> load_from_file(const fs::path& path);

private:
    uint32_t m_id;
};

}