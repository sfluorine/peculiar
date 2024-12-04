#pragma once

#include "vertexbuffer.h"

namespace peculiar
{

class vertex_array_t {
public:
    vertex_array_t(uint32_t id);
    ~vertex_array_t();

    vertex_array_t(const vertex_array_t& other) = delete;
    vertex_array_t& operator=(const vertex_array_t& other) = delete;

    vertex_array_t(vertex_array_t&& other) = delete;
    vertex_array_t& operator=(vertex_array_t&& other) = delete;

    void bind() const;

    void unbind() const;

    void set_index_buffer(const ref_ptr_t<index_buffer_t>& ibo) const;

    void push_vertex_buffer(const ref_ptr_t<vertex_buffer_t>& vbo);

public:
    static ref_ptr_t<vertex_array_t> make();

private:
    uint32_t m_id;
    GLuint m_index { 0 };
};

}