#pragma once

#include "common.h"

namespace peculiar
{

class basic_shader_t {
public:
    basic_shader_t(uint32_t id);
    ~basic_shader_t();

    basic_shader_t(const basic_shader_t& other) = delete;
    basic_shader_t& operator=(const basic_shader_t& other) = delete;

    basic_shader_t(basic_shader_t&& other) = delete;
    basic_shader_t& operator=(basic_shader_t&& other) = delete;

    void bind() const;

    void set_uniform_int1(const char* name, int32_t value) const;
    void set_uniform_int1v(const char* name,
                           GLsizei count,
                           const int32_t* value) const;
    void set_uniform_mat4(const char* name, const glm::mat4& value) const;

public:
    static ref_ptr_t<basic_shader_t> init();

private:
    uint32_t m_id;
};

}