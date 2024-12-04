#include "vertexarray.h"

namespace peculiar
{

vertex_array_t::vertex_array_t(uint32_t id)
    : m_id(id)
{
}

vertex_array_t::~vertex_array_t()
{
    glDeleteVertexArrays(1, &m_id);
}

void vertex_array_t::bind() const
{
    glBindVertexArray(m_id);
}

void vertex_array_t::unbind() const
{
    glBindVertexArray(0);
}

void vertex_array_t::set_index_buffer(
    const ref_ptr_t<index_buffer_t>& ibo) const
{
    bind();
    ibo->bind();
    unbind();
}

void vertex_array_t::push_vertex_buffer(const ref_ptr_t<vertex_buffer_t>& vbo)
{
    bind();
    vbo->bind();

    for (const auto& layout : vbo->get_layouts()) {
        auto layout_offset = reinterpret_cast<const void*>(layout.offset);

        glEnableVertexAttribArray(m_index);

        if (layout.type != GL_FLOAT) {
            glVertexAttribIPointer(m_index++,
                                   layout.component_count,
                                   layout.type,
                                   vbo->get_stride(),
                                   layout_offset);
        } else {
            glVertexAttribPointer(m_index++,
                                  layout.component_count,
                                  layout.type,
                                  layout.normalized,
                                  vbo->get_stride(),
                                  layout_offset);
        }
    }

    unbind();
}

ref_ptr_t<vertex_array_t> vertex_array_t::make()
{
    uint32_t id;
    glGenVertexArrays(1, &id);

    return make_ref<vertex_array_t>(id);
}

}
