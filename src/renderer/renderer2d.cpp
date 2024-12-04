#include "renderer2d.h"
#include "shader.h"
#include "texture.h"
#include "vertexarray.h"

namespace peculiar
{

namespace renderer2d
{

static constexpr uint32_t MAX_TEXTURES = 32;

static constexpr uint32_t MAX_QUAD_COUNT = 12000;
static constexpr uint32_t MAX_QUAD_VERTICES = MAX_QUAD_COUNT * 4;
static constexpr uint32_t MAX_QUAD_INDICES = MAX_QUAD_COUNT * 6;

struct quad_vertex_t {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 color;

    uint32_t tex_index;
};

struct renderer_data_t {
    bool initialized { false };

    ref_ptr_t<basic_shader_t> shader;

    ref_ptr_t<vertex_array_t> quad_vao;
    ref_ptr_t<vertex_buffer_t> quad_vbo;
    ref_ptr_t<index_buffer_t> quad_ibo;

    quad_vertex_t* quad_vertices { nullptr };
    quad_vertex_t* quad_vertices_ptr { nullptr };

    vector_t<ref_ptr_t<texture_t>> textures;

    uint32_t texture_count { 1 }; // 1 because there's a white texture.
    uint32_t quad_index_count { 0 };
};

static renderer_data_t g_render_data {};

static void init_shader_and_buffer_objects(int32_t width, int32_t height)
{
    g_render_data.shader = basic_shader_t::init();

    g_render_data.shader->bind();
    g_render_data.shader->set_uniform_mat4(
        "u_projection",
        glm::ortho(
            0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f));

    g_render_data.quad_vao = vertex_array_t::make();
    g_render_data.quad_vbo
        = vertex_buffer_t::make(MAX_QUAD_VERTICES * sizeof(quad_vertex_t));

    uint32_t offset = 0;
    uint32_t indices[MAX_QUAD_INDICES];
    for (uint32_t i = 0; i < MAX_QUAD_INDICES; i += 6) {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;
        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;
        offset += 4;
    }

    g_render_data.quad_ibo = index_buffer_t::make(sizeof(indices), indices);

    g_render_data.quad_vbo->push_layout(sizeof(float),
                                        3,
                                        GL_FLOAT,
                                        GL_FALSE,
                                        offsetof(quad_vertex_t, position));

    g_render_data.quad_vbo->push_layout(
        sizeof(float), 2, GL_FLOAT, GL_FALSE, offsetof(quad_vertex_t, uv));

    g_render_data.quad_vbo->push_layout(
        sizeof(float), 3, GL_FLOAT, GL_FALSE, offsetof(quad_vertex_t, color));

    g_render_data.quad_vbo->push_layout(sizeof(uint32_t),
                                        1,
                                        GL_UNSIGNED_INT,
                                        GL_FALSE,
                                        offsetof(quad_vertex_t, tex_index));

    g_render_data.quad_vao->push_vertex_buffer(g_render_data.quad_vbo);
    g_render_data.quad_vao->set_index_buffer(g_render_data.quad_ibo);
}

static void init_vertices()
{
    g_render_data.quad_vertices = new quad_vertex_t[MAX_QUAD_VERTICES];
    g_render_data.quad_vertices_ptr = g_render_data.quad_vertices;
}

void init(int32_t width, int32_t height)
{
    if (g_render_data.initialized)
        return;

    init_shader_and_buffer_objects(width, height);
    init_vertices();

    g_render_data.textures.push_back(texture_t::load_white_texture());
    g_render_data.initialized = true;
}

void shutdown()
{
    if (!g_render_data.initialized)
        return;

    delete[] g_render_data.quad_vertices;

    g_render_data.quad_ibo.reset();
    g_render_data.quad_vbo.reset();
    g_render_data.quad_vao.reset();

    g_render_data.shader.reset();

    g_render_data.initialized = false;
}

void begin_drawing()
{
    g_render_data.quad_index_count = 0;
    g_render_data.quad_vertices_ptr = g_render_data.quad_vertices;
}

void end_drawing()
{
    if (g_render_data.quad_index_count) {
        for (uint32_t i = 0; i < g_render_data.texture_count; i++)
            g_render_data.textures[i]->bind(i);

        auto size
            = (g_render_data.quad_vertices_ptr - g_render_data.quad_vertices)
            * sizeof(quad_vertex_t);

        g_render_data.quad_vbo->set_data(size, g_render_data.quad_vertices);

        g_render_data.shader->bind();
        g_render_data.quad_vao->bind();

        glDrawElements(GL_TRIANGLES,
                       g_render_data.quad_index_count,
                       GL_UNSIGNED_INT,
                       nullptr);

        g_render_data.quad_vao->unbind();
    }
}

void draw_quad(glm::vec2 position, glm::vec2 dimension, glm::vec3 color)
{
    if (!g_render_data.initialized)
        return;

    if (g_render_data.quad_index_count >= MAX_QUAD_INDICES) {
        end_drawing();
        begin_drawing();
    }

    g_render_data.quad_vertices_ptr->position
        = glm::vec3(position.x, position.y, 0.0);
    g_render_data.quad_vertices_ptr->uv = glm::vec2(0.0, 1.0);
    g_render_data.quad_vertices_ptr->color = color;
    g_render_data.quad_vertices_ptr->tex_index = 0;
    g_render_data.quad_vertices_ptr++;

    g_render_data.quad_vertices_ptr->position
        = glm::vec3(position.x, position.y + dimension.y, 0.0);
    g_render_data.quad_vertices_ptr->uv = glm::vec2(0.0, 0.0);
    g_render_data.quad_vertices_ptr->color = color;
    g_render_data.quad_vertices_ptr->tex_index = 0;
    g_render_data.quad_vertices_ptr++;

    g_render_data.quad_vertices_ptr->position
        = glm::vec3(position.x + dimension.x, position.y + dimension.y, 0.0);
    g_render_data.quad_vertices_ptr->uv = glm::vec2(1.0, 0.0);
    g_render_data.quad_vertices_ptr->color = color;
    g_render_data.quad_vertices_ptr->tex_index = 0;
    g_render_data.quad_vertices_ptr++;

    g_render_data.quad_vertices_ptr->position
        = glm::vec3(position.x + dimension.x, position.y, 0.0);
    g_render_data.quad_vertices_ptr->uv = glm::vec2(1.0, 1.0);
    g_render_data.quad_vertices_ptr->color = color;
    g_render_data.quad_vertices_ptr->tex_index = 0;
    g_render_data.quad_vertices_ptr++;

    g_render_data.quad_index_count += 6;
}

}

}