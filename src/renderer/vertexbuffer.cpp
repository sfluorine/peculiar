#include "vertexbuffer.h"

namespace peculiar
{

vertex_buffer_t::vertex_buffer_t(uint32_t id)
    : m_id(id)
{
}

vertex_buffer_t::~vertex_buffer_t()
{
    glDeleteBuffers(1, &m_id);
}

void vertex_buffer_t::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void vertex_buffer_t::push_layout(GLsizei size_per_component,
                                  GLint component_count,
                                  GLenum type,
                                  GLboolean normalized,
                                  size_t offset)
{
    m_stride += size_per_component * static_cast<GLsizei>(component_count);
    m_layouts.push_back({ component_count, type, normalized, offset });
}

void vertex_buffer_t::set_data(GLsizeiptr size, const void* data)
{
    bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

ref_ptr_t<vertex_buffer_t> vertex_buffer_t::make(GLsizeiptr size)
{
    uint32_t id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

    return make_ref<vertex_buffer_t>(id);
}

ref_ptr_t<vertex_buffer_t> vertex_buffer_t::make(GLsizeiptr size,
                                                 const void* data)
{
    uint32_t id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return make_ref<vertex_buffer_t>(id);
}

index_buffer_t::index_buffer_t(uint32_t id)
    : m_id(id)
{
}

void index_buffer_t::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

ref_ptr_t<index_buffer_t> index_buffer_t::make(GLsizeiptr size,
                                               const void* data)
{
    uint32_t id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return make_ref<index_buffer_t>(id);
}

}