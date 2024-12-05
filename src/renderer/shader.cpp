#include "shader.h"

namespace peculiar
{

basic_shader_t::basic_shader_t(uint32_t id)
    : m_id(id)
{
    bind();

    int32_t textures[32];
    for (int32_t i = 0; i < 32; i++)
        textures[i] = i;

    set_uniform_int1v("u_textures", 32, textures);
}

basic_shader_t::~basic_shader_t()
{
    glDeleteProgram(m_id);
}

void basic_shader_t::bind() const
{
    glUseProgram(m_id);
}

void basic_shader_t::set_uniform_int1(const char* name, int32_t value) const
{
    auto location = glGetUniformLocation(m_id, name);
    if (location == -1) {
        spdlog::error("could not get uniform location of '{}'", name);
        return;
    }

    glUniform1i(location, value);
}

void basic_shader_t::set_uniform_int1v(const char* name,
                                       GLsizei count,
                                       const int32_t* value) const
{
    auto location = glGetUniformLocation(m_id, name);
    if (location == -1) {
        spdlog::error("could not get uniform location of '{}'", name);
        return;
    }

    glUniform1iv(location, count, value);
}

void basic_shader_t::set_uniform_mat4(const char* name,
                                      const glm::mat4& value) const
{
    auto location = glGetUniformLocation(m_id, name);
    if (location == -1) {
        spdlog::error("could not get uniform location of '{}'", name);
        return;
    }

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

ref_ptr_t<basic_shader_t> basic_shader_t::init()
{
    const char* vcode = R"(
    #version 460 core

    layout (location = 0) in vec3 a_pos;
    layout (location = 1) in vec2 a_uv;
    layout (location = 2) in vec3 a_col;
    layout (location = 3) in uint a_tex_index;

    uniform mat4 u_projection;

    out vec2 v_uv;
    out vec3 v_col;
    out flat uint v_tex_index;

    void main()
    {
        gl_Position = u_projection * vec4(a_pos, 1.0);

        v_uv = a_uv;
        v_col = a_col;
        v_tex_index = a_tex_index;
    }
    )";

    const char* fcode = R"(
    #version 460 core

    out vec4 FragColor;

    uniform sampler2D u_textures[32];

    in vec2 v_uv;
    in vec3 v_col;
    in flat uint v_tex_index;

    void main()
    {
        vec4 tex_color = texture(u_textures[v_tex_index], v_uv);
        if (tex_color.a < 0.1)
            discard;

        FragColor = tex_color;
    }
    )";

    int32_t success = 0;

    auto vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vcode, nullptr);
    glCompileShader(vshader);
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char buffer[256];
        glGetShaderInfoLog(vshader, 256, nullptr, buffer);

        spdlog::critical("vertex shader compile error: {}", buffer);
        exit(EXIT_FAILURE);
    }

    auto fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &fcode, nullptr);
    glCompileShader(fshader);
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char buffer[256];
        glGetShaderInfoLog(fshader, 256, nullptr, buffer);

        spdlog::critical("fragment shader compile error: {}", buffer);
        exit(EXIT_FAILURE);
    }

    auto program = glCreateProgram();
    glAttachShader(program, vshader);
    glAttachShader(program, fshader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        char buffer[256];
        glGetProgramInfoLog(program, 256, nullptr, buffer);

        spdlog::critical("shader program link error: {}", buffer);
        exit(EXIT_FAILURE);
    }

    glDeleteShader(vshader);
    glDeleteShader(fshader);

    return make_ref<basic_shader_t>(program);
}

}