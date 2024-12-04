#pragma once

#include "common.h"

namespace peculiar
{

struct vertex_layout_t {
    GLint component_count;
    GLenum type;
    GLboolean normalized;

    size_t offset;
};

class vertex_buffer_t {
public:
    vertex_buffer_t(uint32_t id);
    ~vertex_buffer_t();

    vertex_buffer_t(const vertex_buffer_t& other) = delete;
    vertex_buffer_t& operator=(const vertex_buffer_t& other) = delete;

    vertex_buffer_t(vertex_buffer_t&& other) = delete;
    vertex_buffer_t& operator=(vertex_buffer_t&& other) = delete;

    GLsizei get_stride() const
    {
        return m_stride;
    }

    const vector_t<vertex_layout_t>& get_layouts() const
    {
        return m_layouts;
    }

    void bind() const;

    void push_layout(GLsizei size_per_component,
                     GLint component_count,
                     GLenum type,
                     GLboolean normalized,
                     size_t offset);

    void set_data(GLsizeiptr size, const void* data);

public:
    static ref_ptr_t<vertex_buffer_t> make(GLsizeiptr size);
    static ref_ptr_t<vertex_buffer_t> make(GLsizeiptr size, const void* data);

private:
    uint32_t m_id;

    GLsizei m_stride { 0 };
    vector_t<vertex_layout_t> m_layouts {};
};

class index_buffer_t {
public:
    index_buffer_t(uint32_t id);

    index_buffer_t(const index_buffer_t& other) = delete;
    index_buffer_t& operator=(const index_buffer_t& other) = delete;

    index_buffer_t(index_buffer_t&& other) = delete;
    index_buffer_t& operator=(index_buffer_t&& other) = delete;

    void bind() const;

public:
    static ref_ptr_t<index_buffer_t> make(GLsizeiptr size, const void* data);

private:
    uint32_t m_id;
};

}